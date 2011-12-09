#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "function.h"


#define PORT 6666

typedef struct sockaddr_in sockaddr_in;

int main(int argc, char** argv)
{
    /// --- declaration des variables
    int serv_sock = 0; ///< Socket serveur
    sockaddr_in serv_addr; ///< Adresse serveur
    int cli_sock = 0; ///< Socket client
    sockaddr_in cli_addr; ///< Adresse client
    int taille_cli_addr = sizeof(cli_addr); ///< Taille de l'adresse client
    int ret = 0; ///< Buffer pour enregistrer les valeurs de retour
    int buffi = 0; ///< Buffer entier general

    char request[65535]; ///< Pour enregistrer la requete


    /// --- creation de la socket
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        return -1;

    /// --- parametrage de la socket (/!\ Fichier de configuration externe)
    serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(PORT);

    /// --- bind socket-adresse
    ret = bind(serv_sock, (sockaddr_in*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
        return -1;

    /// --- ecoute/etablit la connexion
    ret = listen(serv_sock, 5);
    if (ret == -1)
        return -1;

    /// --- traitement des clients
    while (1)
    {
        cli_sock = accept(serv_sock, (struct sockaddr *) &cli_addr, &taille_cli_addr);
        if (cli_sock == -1)
            return -1;

        ret = recv(cli_sock, request, sizeof(request), 0);
        if (ret == -1)
            return -1;

        /// --- get a message from the client and analyze it with httprequest procedure
        printf("%s", request);
       //get_HTTPRequest(request);

        /// --- execute the GPS software

        /// --- reply to the client
    }

    /// --- ferme les sockets
	close(cli_sock);
	close(serv_sock);

    /// --- attend un peu histoire que le client ai le temps de se deco proprement
    sleep(1);

}


