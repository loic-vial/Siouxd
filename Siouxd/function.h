#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

/**
 * Structure contenant les informations de configuration du serveur
 */
struct config_list
{
    char ip_address[32]; ///< L'adresse IP
    int port; ///< Le port d'ecoute
    char web_root_dir[128]; ///< Le nom du repertoire racine
    char log_file[128]; ///< Le nom du fichier de log
    char gps_software[128]; ///< Le nom du programme GPS
    int frequency; ///< La frequence a laquelle on execute le programme (en secondes)
};
typedef struct config_list config_list;

/**
 * Lit un fichier de configuration pour le serveur
 * @param nom_fichier : le nom du fichier de configuration
 * @param config : structure de configuration a remplir (parametre resultat)
 * @return 0 si succes, -1 sinon
 */
int read_configFile(const char* nom_fichier, config_list* config);

/**
 * Lance le programme de localisation GPS a une certaine frequence
 * @param config : la configuration du serveur
 */
void* GPS_refresh(void* config);

/**
 * Traite une simple requete HTTP client (GET <fichier>)
 * @param config : la configuration du serveur
 * @param requete : la requete du client
 * @param reponse : la reponse a envoyer (parametre resultat)
 * @return 0 si succes, -1 sinon
 */
int get_HTTPRequest(const config_list* config, char* requete, char* reponse);

/**
 * Ecrit dans le fichier de log
 * @param config : la configuration du serveur
 * @param chaine : la chaine a ecrire
 * @return 0 si succes, -1 sinon
 */
int write_log(const config_list* config, const char* chaine);


#endif
