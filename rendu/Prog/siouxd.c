#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#include "function.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

/**
 * variable globale servant de signal d'arret pour le serveur
 * le serveur doit s'arreter si global_stop == 1
 */
int global_stop = 0;

/**
 * fonction a appeler lors d'un signal SIGTERM ou SIGINT du systeme d'exploitation
 * pour fermer le serveur proprement
 */
void fermer(int param);

/**
 *****************************************************************************
 *                                siouxd
 *****************************************************************************
 * siouxd est un serveur web leger pour telephone mobile permettant
 * de retrouver la localisation GPS du telephone rapidement
 */
int main()
{
    /// --- declaration des variables
    int serv_sock = 0; ///< Socket serveur
    config_list config; ///< Configuration du serveur (ip, port, fichier log, frequence, etc.)
    sockaddr_in serv_addr; ///< Adresse serveur
    int cli_sock = 0; ///< Socket client
    sockaddr_in cli_addr; ///< Adresse client
    socklen_t taille_cli_addr = sizeof(cli_addr); ///< Taille de l'adresse client
    pthread_t thread_gps; ///< Thread du programme GPS qui tourne en boucle
    pthread_t thread_client; ///< Thread pour chaque nouvelle connexion d'un client
    param_traitement_client config_and_socket; ///< Structure utilisé pour passer en parametre la configuration serveur et la socket client a la fonction "traitement_client"
    char buffer[256] = {0}; ///< Buffer general pour former des chaines de caracteres
    int ret = 0; ///< Buffer pour enregistrer les valeurs de retour

    /// --- parametrage du serveur avec le fichier de configuration
    ret = read_configFile("siouxd.conf", &config);
    if (ret == -1)
    {
        perror(NULL);
        fprintf(stderr, "Erreur : Le fichier \"siouxd.conf\" est introuvable !\n");
        return 1;
    }
    config_and_socket.config = &config;

    /// --- creation de la socket serveur (TCP/IP)
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        perror(NULL);
        fprintf(stderr, "Erreur lors de la création de la socket serveur !\n");
        return 2;
    }

    /// --- parametrage de la socket
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(config.ip_address); ///< Adresse IP
    serv_addr.sin_port = htons(config.port); ///< Port d'ecoute
    serv_addr.sin_family = AF_INET;

    /// --- bind socket-adresse
    ret = bind(serv_sock, (const sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
    {
        perror(NULL);
        fprintf(stderr, "Erreur lors du bind entre la socket et l'adresse !\n");
        fprintf(stderr, "Vous devriez choisir probablement un autre port dans la configuration serveur.\n");
        return 3;
    }

    /// --- ecoute/etablit la connexion
    ret = listen(serv_sock, 5);
    if (ret == -1)
    {
        perror(NULL);
        fprintf(stderr, "Erreur lors du listen !\n");
        return 4;
    }

    /// --- ecrit dans le fichier de log que le serveur est lancé
    sprintf(buffer, "serveur lancé avec l'adresse IP %s, en ecoute sur le port %d", config.ip_address, config.port);
    write_log(&config, buffer);

    /// --- lance le programme de localisation GPS dans un thread separé
    pthread_create(&thread_gps, NULL, GPS_refresh, (void*)&config);

    /// --- associe le signal SIGTERM et SIGINT (fermeture du programme) a la fonction "fermer"
    signal(SIGTERM, fermer);
    signal(SIGINT, fermer);

    /// --- traitement des clients
    while (global_stop != 1)
    {
        /// --- ouvre un socket client
        cli_sock = accept(serv_sock, (sockaddr*)&cli_addr, &taille_cli_addr);
        if (cli_sock == -1)
        {
            fprintf(stderr, "Erreur lors de l'ouverture d'une socket client");
            continue;
        }

        sprintf(buffer, "connexion d'un client sur la socket %d depuis l'adresse %s avec le port %d", cli_sock, inet_ntoa(cli_addr.sin_addr), htons(cli_addr.sin_port));
        write_log(&config, buffer);

        /// --- traitement du client dans un thread séparé
        config_and_socket.socket_client = cli_sock;
        pthread_create(&thread_client, NULL, traitement_client, (void*)&config_and_socket);
    }

    /// --- attend la fermeture du dernier thread client
    pthread_join(thread_client, NULL);

    /// --- ferme le thread du programme GPS
    pthread_cancel(thread_gps);

    /// --- ferme la socket serveur
    close(serv_sock);

    write_log(&config, "arret du serveur");

    /// --- retour 0 = tout s'est bien passé
    return 0;
}

void fermer(int param)
{
    printf("Reception du signal d'arret %d : le serveur se fermera a la prochaine connexion d'un client.\n", param);
    global_stop = 1;
}

