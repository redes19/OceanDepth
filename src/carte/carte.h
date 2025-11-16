#ifndef CARTE_H
#define CARTE_H

typedef struct Plongeur Plongeur;

typedef struct {
    char nom[30];
    int profondeur;
    int ennemis;
    int tresor;
} Zone;

Zone **newCarte();
void printCarte(Zone **zone);
int printZone(Plongeur *plongeur, Zone **zone);
void deplacement(Plongeur *joueur,Zone **carte);


#endif // CARTE_H
