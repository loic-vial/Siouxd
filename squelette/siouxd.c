/* Name: siouxd.c */
/*  Function: main program for the web server*/

#include "function.h" 
#define PORT 0x1234



/*  Main Function*/
void main(argc, argv)
int argc; char **argv;
{
  /* Variable declaration and initialization */
  char  request[100]="";
  int nb_lu =0;
  int nb = 0;
  char *buffer;

  /*//////////////////////////////////////////*/
  /* Given code BEGIN					  */
  /*//////////////////////////////////////////*/

	int sd, sd_current ;
	int 	 addrlen;
  	struct sockaddr_in serv_addr, cli_addr;

  

  /*  Read and process the configuration file */
  /*  and gather all parameters like IP address, port, ... */

  //////////////////////////////////
  /* Network initialization BEGIN */
  //////////////////////////////////


  /* Get an internet domain socket opened */

 
  /* Complete the socket structure with parameters from the config file*/
  
  /* Fill socket */
  
  /* bind the socket to the port number */
  
  /* Listen x connection */ 

  /* wait for a client to talk to us */


  /* log the client connexion with the date its ip adress and its port */

       
   printf("Hi there, from  %s#\n",inet_ntoa(cli_addr.sin_addr));
   printf("Coming from port %d\n",ntohs(cli_addr.sin_port));

/* get a message from the client and analyze it with httprequest procedure */

 /* execute the GPS software */ 
 
 /* reply to the client */ 
  
/* close up both sockets */
	close(sd_current); close(sd);
        
        /* give client a chance to properly shutdown */
        sleep(1);
}

 
