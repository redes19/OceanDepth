#include "carte.h";
#include "../joueur/joueur.h";

#include <stdio.h>
#include <stdlib.h>

Zone *addZone() {
    Zone *zone = malloc(sizeof(Zone)*4);
    zone[0] = (Zone){"Surface", 0, 0, 0, 1};
    zone[1] = (Zone){"Récif Corallien", 50, 3, 0, 0};
    zone[2] = (Zone){"Épave Engloutie", 150, 5, 1, 0};
    zone[3] = (Zone){"Grotte Marine", 200, 0, 0, 1};
    return zone;
}

int printZone(Plongeur *plongeur) {
    printf("Vous etes dans la zone : %s",plongeur->zone);
    printf("Quels action voulez-vous effectuer (taper le chiffre)\n"
        "1 - Se deplacer\n"
        "2 - Explorer la zone\n"
        "3 - Retour surface\n"
        "4 - Carte detaillee\n");
    int action = 0;
    scanf("%d",&action);
    return action;
}
