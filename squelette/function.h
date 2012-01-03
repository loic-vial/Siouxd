#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* standard librarie */
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
 
#define BUFFER_LENGHT 1024

struct config_list { 
/* structure in which parameters read in config file will be filled */
  char * ip_address;
  int port;
  char * web_root_dir;
  char * log_file;
  char * gps_software;
  int frequency;
} config;





/********************************function GPS_refresh*************************************************************/
/* Name: GPS_refresh.c
  Function: launch every x seconds a specific GPS program
*/
void GPS_refresh (int);

/**************************************************************************************************/

/********************************function read_configFile*************************************************************/
/* Function: read configuration file and gather parameters for the Web server
  input: char for config file
  output: struct containing all parameters
*/

void read_configFile(char *);
/**************************************************************************************************/



/********************************function get_HTTPRequest*************************************************************/
/* Function: process the HTTP client request (GET <file.html>) */

void get_HTTPRequest (char *);
/**************************************************************************************************/



/********************************function log*************************************************************/
/* Function: Log the start stop and access of the server */


void log(char *);
/**************************************************************************************************/



