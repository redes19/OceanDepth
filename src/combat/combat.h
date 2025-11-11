#ifndef COMBAT_H
#define COMBAT_H

#include "../creature/creature.h"
#include "../joueur/joueur.h"

typedef enum {
    ENT_PLONGEUR,
    ENT_CREATURE,
} EntityType;

typedef struct {
    EntityType type;
    union {
        Plongeur plongeur;
        int creature_id;
    } u;
} Entity;


void initFight(Plongeur *plongeur, int depth);

#endif // COMBAT_H