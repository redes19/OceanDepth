#ifndef JOUEUR_H
#define JOUEUR_H

#include "../carte/carte.h"
#include "../creature/creature.h"

typedef void(*CompAquatique)(Plongeur *plongeur, void *target);

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
    CompAquatique comp[3];

    Zone zone;
} Plongeur;

// compétences aquatiques
void apneeProlongee(Plongeur *plongeur, void *target);
void dechargeElectrique(Plongeur *plongeur, void *target);
void communicationMarine(Plongeur *plongeur, void *target);

void displayCompAquatique();

Plongeur *initializePlongeur(Zone zone);
void printPlongeur(Plongeur *plongeur);
void perteO2(Plongeur *plongeur);


#endif // JOUEUR_H
