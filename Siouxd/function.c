#include <pthread.h>
#include "function.h"

void* GPS_refresh(void* void_config)
{
    /// --- recupere la configuration serveur
    config_list* config = (config_list*)void_config;

    /// --- lance le programme en boucle
    while (1)
    {
        printf("Lancement du programme GPS : %s\n", config->gps_software);
        if (system(config->gps_software) != 0)
	    printf("Erreur lors de l'execution du programme GPS\n");
        sleep(config->frequency);
    }
}

int read_configFile(const char* nom_fichier, config_list* config)
{
    /// --- ouverture du fichier en lecture seule
    FILE* fichier = fopen(nom_fichier, "r");

    /// --- si l'ouverture a echoue, on retourne -1
    if (fichier == NULL)
        return -1;

    /// --- lecture du fichier de configuration
    ///?/ Controle des configurations!
    ///   controle ip adresse sur 16 bits => sinon return -1
    ///   controle web_root_dir, faut pas de slash en fin, sinon on l'enleve
    fscanf(fichier,
           "ip_address %s port %d web_root_dir %s log_file %s gps_software %s frequency %d",
           config->ip_address,
           &(config->port),
           config->web_root_dir,
           config->log_file,
           config->gps_software,
           &(config->frequency));

    /// --- affichage de la configuration (debug)
    /*
    printf("Configuration du serveur : \n\n");
    printf("- ip_address : %s\n", config->ip_address);
    printf("- port : %d\n", config->port);
    printf("- web_root_dir : %s\n", config->web_root_dir);
    printf("- log_file : %s \n", config->log_file);
    printf("- gps_software : %s \n", config->gps_software);
    printf("- frequency : %d \n", config->frequency);
    */


    /// --- fermeture du fichier
    fclose(fichier);
    return 0;

}

int get_HTTPRequest(const config_list* config, char* requete, char* reponse)
{
    /// --- declare les variables
    char* requete_cmd = strtok(requete, " "); ///< premier parametre de la requete
    char* requete_fichier = strtok(NULL, " "); ///< second parametre de la requete
    char buffer[1024] = {0}; ///< buffer
    FILE* fichier = NULL; ///< le fichier demandé
    char contenu_fichier[65535] = {0}; ///< le contenu du fichier
    int taille_fichier = 0; ///< la taille du fichier

    /// --- verifie la validite de la requete (GET <fichier>)
    if (requete_cmd == 0 || strcmp(requete_cmd, "GET") != 0 || requete_fichier == 0)
        return -1;

    /// --- ecrit dans le fichier de log le fichier demandé
    sprintf(buffer, "demande du fichier %s", requete_fichier);
    write_log(config, buffer);

    /// --- recupere le chemin absolu vers le fichier demandé
    /// --- * controle du cas ou le fichier qu'on demande est "/" (racine du serveur)
    if (strcmp(requete_fichier, "/") == 0)
        sprintf(buffer, "%s/index.html", config->web_root_dir);
    /// --- * controle du cas general
    else
        sprintf(buffer, "%s%s", config->web_root_dir, requete_fichier);

    /// --- ouvre le fichier
    fichier = fopen(buffer, "r");
    if (fichier == NULL)
        return -1;

    /// --- lit le fichier et enregistre son contenu
    while (fgets(buffer, 1024, fichier) != NULL)
        strcat(contenu_fichier, buffer);
    taille_fichier = ftell(fichier);

    /// --- construit la reponse
    sprintf(reponse, "HTTP/1.0 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: %d\r\n\
\r\n\
%s", taille_fichier, contenu_fichier);

    /// --- ferme le fichier
    fclose(fichier);

    return 0;
}

int write_log(const config_list* config, const char* chaine)
{
    /// --- declare les variables
    char str_fichier_log[256] = {0}; ///< chemin absolu vers le fichier de log
    FILE* fichier_log = NULL; ///< fichier de log
    time_t timestamp = time(NULL); ///< timestamp courant
    char* str_timestamp = ctime(&timestamp); ///< timestamp courant sous forme de string
    char message[512]  = {0}; ///< message a ecrire (avec la date + saut de ligne)

    /// --- recupere le chemin absolu vers le fichier de log
    strcat(str_fichier_log, config->web_root_dir);
    strcat(str_fichier_log, "/");
    strcat(str_fichier_log, config->log_file);

    /// --- ouvre le fichier de log en mode "ajout"
    fichier_log = fopen(str_fichier_log, "a");
    if (fichier_log == NULL)
        return -1;

    /// --- ajoute la date et un saut de ligne au message
    strcat(message, str_timestamp); ///< ajoute la date
    *strchr(message, '\n') = 0; ///< supprime le saut de ligne de la fonction ctime
    strcat(message, " : ");
    strcat(message, chaine); ///< ajoute la chaine a ecrire
    strcat(message, "\n"); ///< saut de ligne

    /// --- ecrit le message dans le fichier
    fputs(message, fichier_log);

    /// --- ferme le fichier
    fclose(fichier_log);

    return 0;
}
