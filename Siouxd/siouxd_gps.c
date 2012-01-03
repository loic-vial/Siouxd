#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Ce programme genere aleatoirement une position GPS
 * et l'enregistre dans le fichier position.html
 */
int main()
{
     srand(time(NULL));

    /// 1) calcul de latitude aleatoire

    /// --- generation d'un nombre entre 0 et 90
    int latitude = rand() % 90;

    /// --- generation d'un signe (1 ou -1)
    int signe = rand() % 2 == 0 ? -1 : 1;

    /// --- application du signe sur le nombre (-90 - 90)
    latitude *= signe;


    /// 2) calcul de longitude aleatoire

    /// --- generation d'un nombre entre 0 et 180
    int longitude = rand() % 180;


    /// 3) ecriture de la position GPS dans le fichier
    char html[1024];

    sprintf(html, "\
<html>\
            <head>\
                <title>GPS</title>\
            </head>\
            <body style=\"text-align: center;\">\
                <br /><br />\
                <p style=\"text-align: center;\">\
                    <h1>Latitude : <b>%d</b></h1>\
                    <h1>Longitude : <b>%d</b></h1>\
                </p>\
            </body>\
</html>", latitude, longitude);

    FILE* fichier_html = fopen("position.html", "w");

    if (fichier_html == NULL)
        return -1;

    fputs(html, fichier_html);

    fclose(fichier_html);

    return 0;
}
