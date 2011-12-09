
/* function.c */
#include "function.h"



/********************************function GPS_refresh***********************/
/* Name: GPS_refresh.c
Function: launch every x seconds a specific GPS program
*/



void GPS_refresh (int frequency) {
  /* infinite loop to launch the GPS program every "frequency" seconds */


}

/********************************function read_configFile*********************/

/* Function: read configuration file and gather parameters for the Web server */
/* input: char for config file */
/* output: struct containing all parameters */

void read_configFile(char *namefile){




}

/******************function get_HTTPRequest******/
/* Function: process the HTTP cmlient request (GET <file.html>)*/
void get_HTTPRequest(char * request){

  /*/////////////////*/
  /* Read file BEGIN */
  /*/////////////////*/
  /* Variable declaration and intialization */

  int fd = 0;
  int nb = 0;
  char *buffer;
  int nbCharLu = 0;
  int initStat = 0 ;
  char nb_caractere = NULL;
  char request_cmd = NULL;
  char request_htm = NULL;

  /* Test if 2 arguments : GET <file> */
  /* get request command and file */
  request_cmd = (char *) strtok (request," ");
  request_htm = (char *) strtok (NULL," ");

  /* set the absolute path for the requested html file */
    printf("%s", request);
  /* Syntaxe test of the request */


  /* Test if file is valid and not a directory */


  /* number of character in the file */

  /* Read the file */


  /* write into network file descriptor to send data to client */




}



/********************************function log*************************************************************/
/* Function: Log the start stop and access of the server */




void log(char * line_to_write) {
  /* variable declaration and initialization */

  int log_file_fd = 0;
  time_t time;
  char * message[BUFFER_LENGHT];

  /* open log file in write only mode */



  /* get system date */


  /* get string to put in log file  with the format time and message */


  /* write in log file */


  /* close log file */
}
