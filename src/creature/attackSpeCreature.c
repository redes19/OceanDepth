#include <stdio.h>

#include "creature.h"
#include "../joueur/joueur.h"

int is_poisoned = 2;
int is_paralysed = 0;

void poisonBite(CreatureMarine *creature, void *target) {
    Plongeur *plongeur = (Plongeur *)target;
    if (plongeur->effect){
        printf("L'effet echoue\n");
        return;
    }

    int damage = (creature->max_attack + creature->min_attack) / 2;
    printf("Vous subissez l'effet Speciale PoisonBite de %s\n", creature->name);
    printf("Vous perdez %d de point de vie!\n", damage);

    plongeur->points_de_vie -= damage;
    plongeur->effect = POISONED;
    is_poisoned = 0;
}

void frenesieSanguinaire(CreatureMarine *creatureMarine, void *target) {
    (void)target; // évite -Wunused-parameter
    if (creatureMarine->life < 50 ) {
        creatureMarine->max_attack += (creatureMarine->max_attack * 30) / 100;
    }
}

void carapace(CreatureMarine *creature, void *target) {
    (void)target;
    if (creature->effect){
        printf("L'effet echoue\n");
        return;
    }

    creature->effect = PROTECTED;
}

void chargePerforante(CreatureMarine *creature, void *target) {
    (void)target;
    Plongeur *plongeur = (Plongeur *)target;

    plongeur->points_de_vie -= creature->max_attack;
}

void confusion(CreatureMarine *creature, void *target) {
    (void)creature;
    Plongeur *plongeur = (Plongeur *)target;
    if (plongeur->effect){
        printf("L'effet echoue\n");
        return;
    }

    plongeur->effect = PARALYSED;
    is_paralysed = 1;
}
