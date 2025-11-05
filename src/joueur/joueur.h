#ifndef JOUEUR_H
#define JOUEUR_H

#include "../carte/carte.h"
#include "../creature/creature.h"


typedef struct Plongeur {
    int points_de_vie;
    int points_de_vie_max;
    int attack;
    int vitesse;
    float defense;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue;
    int perles;
    Effect effect;

    Zone zone;
} Plongeur;

Plongeur *initializePlongeur(Zone zone);
void printPlongeur(Plongeur *plongeur);
void perteO2(Plongeur *plongeur);


#endif // JOUEUR_H
