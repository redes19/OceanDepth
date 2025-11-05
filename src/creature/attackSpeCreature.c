#include <stdio.h>

#include "creature.h"
#include "../joueur/joueur.h"

void poisonBite(CreatureMarine *creatureMarine, void *target) {
    Plongeur *plongeur = (Plongeur *)target;
    int damage = (creatureMarine->max_attack + creatureMarine->min_attack) / 2;
    printf("Vous subissez l'effet Speciale PoisonBite de %s\n", creatureMarine->name);
    printf("Vous perdez %d de point de vie!\n", damage);

    plongeur->points_de_vie -= damage;
    plongeur->effect = POISONED;
}

void frenesieSanguinaire(CreatureMarine *creatureMarine, void *target) {
    (void)target; // évite -Wunused-parameter
    if (creatureMarine->life < 50 ) {
        creatureMarine->max_attack += (creatureMarine->max_attack * 30) / 100;
    }
}

void carapace(CreatureMarine *creature, void *target) {
    (void)target; // évite -Wunused-parameter
    if (creature->effect) printf("%s a deja un effet et ne peux en cumuler\n", creature->name);

    creature->effect = PROTECTED;
}

void chargePerforante(CreatureMarine *creature, void *target) {
    (void)target; // évite -Wunused-parameter
    Plongeur *plongeur = (Plongeur *)target;

    plongeur->points_de_vie -= creature->max_attack;
}

void confusion(CreatureMarine *creature, void *target) {
    (void)creature;
    Plongeur *plongeur = (Plongeur *)target;

    plongeur->effect = REDUCEDDAMAGE;
}