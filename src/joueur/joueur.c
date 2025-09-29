#include <stdio.h>
#include <stdlib.h>
#include "joueur.h"


Plongeur *initializePlongeur() {
    Plongeur *plongeur = malloc(sizeof(Plongeur));
    plongeur->points_de_vie = 100;
    plongeur->points_de_vie_max = 100;
    plongeur->niveau_oxygene = 100;
    plongeur->niveau_oxygene_max = 100;
    plongeur->niveau_fatigue = 0;
    plongeur->perles = 0;
    return plongeur;
}

void printPlongeur(Plongeur *plongeur, int profondeur) {
    printf("OceanDepths - Profondeur: %dm      Perle: %d\n",profondeur,plongeur->perles);
    printf("Vie : [");
    for (int i = 0; i < plongeur->points_de_vie_max; i++ ) {
        if (plongeur->points_de_vie >= i) {
            printf("|");
        }else {
            printf("-");
        }
    }
    printf("] %d/%d\n",plongeur->points_de_vie,plongeur->points_de_vie_max);
    printf("Oxygene : [");
    for (int i = 0; i < plongeur->niveau_oxygene_max; i++ ) {
        if (plongeur->niveau_oxygene >= i) {
            printf("|");
        }else {
            printf("-");
        }
    }
    printf("] %d/%d\n",plongeur->niveau_oxygene,plongeur->niveau_oxygene_max);
    printf("Fatigue : [");
    for (int i = 0; i < 6; i++ ) {
        if (plongeur->niveau_fatigue <= i) {
            printf("|");
        }else {
            printf("-");
        }
    }
    printf("] %d/5\n",plongeur->niveau_fatigue);
}