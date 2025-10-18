#ifndef JOUEUR_H
#define JOUEUR_H

#include "../carte/carte.h"

typedef struct EffectAttack {
    // code
} EffectAttack;

typedef struct Plongeur {
    int points_de_vie;
    int points_de_vie_max;
    int attack;
    EffectAttack efectAttack;
    int vitesse;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue;
    int perles;
    Zone zone;
} Plongeur;

Plongeur *initializePlongeur(Zone zone);
void printPlongeur(Plongeur *plongeur);
void perteO2(Plongeur *plongeur);


#endif // JOUEUR_H
