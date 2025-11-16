#ifndef CARTE_H
#define CARTE_H
#include "../inventaire/inventaire.h"
typedef struct Plongeur Plongeur;

typedef struct {
    char nom[30];
    int profondeur;
    int ennemis;
    int tresor;
} Zone;

Zone **newCarte();
void printCarte(Plongeur *plongeur, Zone **zone);
int printZone(Plongeur *plongeur, Zone **zone);
void deplacement(Plongeur *joueur,Zone **carte);
int a_la_carte(Inventaire *inv, int niveau_cible);

#endif // CARTE_H
