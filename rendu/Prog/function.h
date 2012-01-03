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
    char ip_address[16]; ///< L'adresse IP du serveur
    int port; ///< Le port d'ecoute
    char web_root_dir[64]; ///< Le chemin vers le repertoire racine (ou se trouvent les fichiers html)
    char log_file[64]; ///< Le chemin vers le fichier de log
    char gps_software[64]; ///< Le programme GPS a lancer
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
 * Structure servant de parametre a la fonction "traitement_client" definie en dessous (fonction threadé)
 */
struct param_traitement_client
{
    config_list* config;
    int socket_client;
};
typedef struct param_traitement_client param_traitement_client;

/**
 * Traite la connexion d'un client
 * @param config_and_socket : la structure contenant la configuration serveur et la socket du client a traiter
 */
void* traitement_client(void* config_and_socket);

/**
 * Traite une simple requete HTTP client (GET <fichier>)
 * @param config : la configuration du serveur
 * @param requete : la requete du client
 * @param reponse : la reponse a envoyer (parametre resultat)
 * @return 0 si succes, -1 si la requete est invalide, -2 si le fichier demandé n'existe pas
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
