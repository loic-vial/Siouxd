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
    /// --- creation de la socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (socket == -1)
        return -1;

    /// --- parametrage de la socket
    sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(PORT);

    /// --- bind socket-port
    int ret = bind(sock, (sockaddr_in*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
        return -1;

    /// --- ecoute/etablit la connexion
    ret = listen(sock, 5);
    if (ret == -1)
        return -1;

    /// --- traitement des clients
    sockaddr_in cli_addr;
    int addrlen;
    int sock_client;

    addrlen = sizeof(cli_addr);
    sock_client = accept(sd, (struct sockaddr *)  &cli_addr, &addrlen);
    if (sock_client == -1)
        printf("Erreur");








/*





  // Variable declaration and initialization
  char  request[100];
  int nb_lu =0;
  int nb = 0;
  char *buffer;

  /*//////////////////////////////////////////*/
  /* Given code BEGIN					  */
 // //////////////////////////////////////////
/*
	int sd, sd_current ;
	int 	 addrlen;
  	struct sockaddr_in serv_addr, cli_addr;



  /*  Read and process the configuration file */
  /*  and gather all parameters like IP address, port, ... */

  //////////////////////////////////
  /* Network initialization BEGIN */
  //////////////////////////////////


  /* Get an internet domain socket opened *//*
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

  /* Complete the socket structure with parameters from the config file*//*
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

  /* Fill socket */

  /* bind the socket to the port number *//*
	if (bind(sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		perror("bind");
		exit(1);
	}else
		printf("socket binded\n");

  /* Listen x connection *//*
	if (listen(sd, 5) == -1) {
		perror("listen");
		exit(1);
	}else printf("listening\n");

  /* wait for a client to talk to us *//*
        addrlen = sizeof(cli_addr);
	if ((sd_current = accept(sd, (struct sockaddr *)  &cli_addr, &addrlen)) == -1) {
		perror("accept");
		exit(1);
	}else printf("accepted\n");

  /* log the client connexion with the date its ip adress and its port *//*
   printf("Hi there, from  %s#\n",inet_ntoa(cli_addr.sin_addr));
   printf("Coming from port %d\n",ntohs(cli_addr.sin_port));

/* get a message from the client and analyze it with httprequest procedure *//*
	if (recv(sd_current, request, sizeof(request), 0) == -1) {
		perror("recv");
		exit(1);
	}

   get_HTTPRequest(request);

 /* execute the GPS software */

 /* reply to the client */

/* close up both sockets *//*
	close(sd_current); close(sd);

        /* give client a chance to properly shutdown */
        sleep(1);
}


