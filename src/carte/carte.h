#ifndef CARTE_H
#define CARTE_H

typedef struct Plongeur Plongeur;

typedef struct {
    char nom[30];
    int profondeur;
    int ennemis;
    int tresor;
    int sure;
} Zone;

Zone *addZone();
int printZone(Plongeur *plongeur);

#endif // CARTE_H
