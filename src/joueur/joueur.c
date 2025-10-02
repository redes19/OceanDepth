#include <stdio.h>
#include <stdlib.h>
#include "joueur.h"


Plongeur *initializePlongeur(Zone zone) {
    Plongeur *plongeur = malloc(sizeof(Plongeur));
    plongeur->points_de_vie = 100;
    plongeur->points_de_vie_max = 100;
    plongeur->niveau_oxygene = 100;
    plongeur->niveau_oxygene_max = 100;
    plongeur->niveau_fatigue = 0;
    plongeur->perles = 0;
    plongeur->zone = zone;
    return plongeur;
}

void printPlongeur(Plongeur *plongeur) {
    printf("OceanDepths - Zone: %s      Profondeur: %dm      Perle: %d\n",plongeur->zone.nom,plongeur->zone.profondeur,plongeur->perles);
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
    // Boucle qui parcourt de 0 jusqu’au niveau maximum d’oxygène du plongeur
    for (int i = 0; i < plongeur->niveau_oxygene_max; i++) {

        // Si le niveau d’oxygène actuel est supérieur à l’index i
        // -> On affiche une barre pleine (|)
        if (i < plongeur->niveau_oxygene) {
            printf("|");
        }
        // Sinon (l’oxygène restant est inférieur à i)
        // -> On affiche une barre vide (-)
        else {
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

void perteO2(Plongeur *plongeur) {
    if (plongeur->zone.profondeur<=100) {
        if (plongeur->niveau_oxygene >=2) {
            plongeur->niveau_oxygene -=2;
        }else {
            plongeur->points_de_vie -=5;
        }
    }else if (plongeur->zone.profondeur<=200) {
        if (plongeur->niveau_oxygene >=3) {
            plongeur->niveau_oxygene -=3;
        }else {
            plongeur->points_de_vie -=5;
        }
    }else if (plongeur->zone.profondeur<=300) {
        if (plongeur->niveau_oxygene >=4) {
            plongeur->niveau_oxygene -=4;
        }else {
            plongeur->points_de_vie -=5;
        }
    }else if (plongeur->zone.profondeur > 300) {
        if (plongeur->niveau_oxygene >=5) {
            plongeur->niveau_oxygene -=5;
        }else {
            plongeur->points_de_vie -=5;
        }
    }
}