#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "function.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

int main(int argc, char** argv)
{
    /// --- declaration des variables
    int serv_sock = 0; ///< Socket serveur
    config_list config; ///< Configuration du serveur (ip, port, fichier log, frequence, etc.)
    sockaddr_in serv_addr; ///< Adresse serveur
    int cli_sock = 0; ///< Socket client
    sockaddr_in cli_addr; ///< Adresse client
    socklen_t taille_cli_addr = sizeof(cli_addr); ///< Taille de l'adresse client
    pthread_t thread_gps; ///< Thread du programme GPS qui tourne en boucle
    char requete[65535] = {0}; ///< Requete du client
    char reponse[65535] = {0}; ///< Reponse a envoyer au client
    int ret = 0; ///< Buffer pour enregistrer les valeurs de retour

    /// --- parametrage du serveur avec le fichier de configuration
    ret = read_configFile("siouxd.conf", &config);
    if (ret == -1)
        return 1;

    ret = write_log(&config, "demarrage du serveur");
    if (ret == -1)
        return 2;

    /// --- creation de la socket serveur (TCP/IP)
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        return 3;

    /// --- parametrage de la socket
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(config.ip_address); ///< Adresse IP d'ecoute
    serv_addr.sin_port = htons(config.port); ///< Port d'ecoute
    serv_addr.sin_family = AF_INET;

    /// --- bind socket-adresse
    ret = bind(serv_sock, (const sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
        return 4;

    /// --- ecoute/etablit la connexion
    ret = listen(serv_sock, 5);
    if (ret == -1)
        return 5;

    /// --- lance le programme de localisation GPS en boucle
    ret = pthread_create(&thread_gps, NULL, GPS_refresh, (void*)&config) != 0;
    if (ret != 0)
        return 6;

    /// --- traitement des clients
    while (1)
    {
        /// --- ouvre un socket client
        cli_sock = accept(serv_sock, (sockaddr*)&cli_addr, &taille_cli_addr);
        if (cli_sock == -1)
        {
            write_log(&config, "erreur lors de l'ouverture du socket client");
            continue;
        }

        /// --- recupere la requete du client
        ret = recv(cli_sock, requete, sizeof(requete), 0);
        if (ret == -1)
        {
            write_log(&config, "erreur lors de la reception de la requete client");
            close(cli_sock);
            continue;
        }

        /// --- analyse la requete et forme la reponse
        ret = get_HTTPRequest(&config, requete, reponse);
        if (ret == -1)
        {
            write_log(&config, "requete invalide de la part du client");
            sprintf(reponse, "HTTP/1.0 404 Not Found\r\n\
Content-Type: text/html\r\n\
Content-Length: 9\r\n\
\r\n\
Not Found");
        }

        /// --- repond au client
        ret = send(cli_sock, reponse, strlen(reponse), 0);
        if (ret == -1)
            write_log(&config, "erreur lors de l'envoi de la reponse");

        /// --- fin de transmission
        close(cli_sock);
    }

    /// --- ferme les sockets
    write_log(&config, "arret du serveur");
    close(cli_sock);
	close(serv_sock);

    /// --- retour 0 = tout s'est bien passe
    return 0;
}
