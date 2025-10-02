#ifndef JOUEUR_H
#define JOUEUR_H
#include "../carte/carte.h"

typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue;
    int perles;
    Zone zone;
} Plongeur;
#endif // JOUEUR_H
