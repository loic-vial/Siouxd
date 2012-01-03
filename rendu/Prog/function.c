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
	        fprintf(stderr, "Erreur lors de l'execution du programme GPS\n");
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
    fscanf(fichier, "ip_address %s ", config->ip_address);
    fscanf(fichier, "port %d ", &(config->port));
    fscanf(fichier, "web_root_dir %s ", config->web_root_dir);
    fscanf(fichier, "log_file %s ", config->log_file);
    fscanf(fichier, "gps_software %s ", config->gps_software);
    fscanf(fichier, "frequency %d ", &(config->frequency));

    /// --- fermeture du fichier
    fclose(fichier);
    return 0;

}

void* traitement_client(void* void_config_and_socket)
{
    /// --- recupere les parametres et declare les variables
    param_traitement_client* config_and_socket = (param_traitement_client*)void_config_and_socket;
    const config_list* config = config_and_socket->config; ///< configuration du serveur
    const int cli_sock = config_and_socket->socket_client; ///< socket du client
    char requete[65535] = {0}; ///< requete du client
    char reponse[65535] = {0}; ///< reponse a envoyer au client
    char buffer[256] = {0}; ///< Buffer general pour former des chaines de caracteres
    int ret = 0; ///< buffer pour enregistrer les valeurs de retour

    /// --- recupere la requete du client
    ret = recv(cli_sock, requete, sizeof(requete), 0);
    if (ret == -1)
    {
        write_log(config, "erreur lors de la reception d'une requete client");
        close(cli_sock);
        return NULL;
    }

    /// --- analyse la requete et forme la reponse
    ret = get_HTTPRequest(config, requete, reponse);
    sprintf(buffer, "requete \"%s\" de la part du client sur la socket %d", requete, cli_sock);
    write_log(config, buffer);
    if (ret == -1)
        write_log(config, "requete invalide");
    else if (ret == -2)
        write_log(config, "fichier non trouvé");

    /// --- repond au client
    ret = send(cli_sock, reponse, strlen(reponse), 0);
    if (ret == -1)
        write_log(config, "erreur lors de l'envoi d'une reponse");

    /// --- fin de transmission
    close(cli_sock);
    return NULL;
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
    {
        sprintf(reponse, "HTTP/1.0 400 Bad Request\r\n\
Content-Type: text/html\r\n\
Content-Length: 11\r\n\
\r\n\
Bad Request");
        return -1;
    }

    /// --- reecrit la requete pour la simplifier au retour
    sprintf(requete, "GET %s", requete_fichier);

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
    {
        sprintf(reponse, "HTTP/1.0 404 Not Found\r\n\
Content-Type: text/html\r\n\
Content-Length: 9\r\n\
\r\n\
Not Found");
        return -2;
    }

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
    FILE* fichier_log = NULL; ///< fichier de log
    time_t timestamp = time(NULL); ///< timestamp courant
    char* str_timestamp = ctime(&timestamp); ///< timestamp courant sous forme de string
    char message[512]  = {0}; ///< message a ecrire (avec la date + saut de ligne)

    printf("Ecriture dans le fichier de log : %s\n", config->log_file);

    /// --- ouvre le fichier de log en mode "ajout"
    fichier_log = fopen(config->log_file, "a");
    if (fichier_log == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de log\n");
        return -1;
    }

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
