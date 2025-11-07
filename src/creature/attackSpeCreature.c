#include <stdio.h>

#include "creature.h"
#include "../joueur/joueur.h"

int is_poisoned = 2;
int is_paralysed = 0;
int is_protected = 2;

void poisonBite(CreatureMarine *creature, void *target) {
    Plongeur *plongeur = (Plongeur *)target;
    if (plongeur->effect != NO_EFFECT){
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

void frenesieSanguinaire(CreatureMarine *creature, void *target) {
    (void)target; // évite -Wunused-parameter
    if (creature->life < 50 ) {
        creature->max_attack += (creature->max_attack * 30) / 100;
    }

    printf("%s utilise frenesie sanguinaire et gagne en puissance\n",creature->name);
}

void carapace(CreatureMarine *creature, void *target) {
    (void)target;
    if (creature->effect){
        printf("L'effet echoue\n");
        return;
    }
    printf("%s utilise carapace et se protège pendant 2 tours\n", creature->name);

    is_protected = 0;
    creature->effect = PROTECTED;
}

void chargePerforante(CreatureMarine *creature, void *target) {
    Plongeur *plongeur = (Plongeur *)target;
    printf("%s utilise charge perforante sur vous\n", creature->name);
    printf("Cette attaque traverse votre defense et vous inflige %d de degats\n", creature->max_attack);

    plongeur->points_de_vie -= creature->max_attack;
}

void confusion(CreatureMarine *creature, void *target) {
    (void)creature;
    Plongeur *plongeur = (Plongeur *)target;
    if (plongeur->effect){
        printf("L'effet echoue\n");
        return;
    }

    printf("%s utilise confusion, cette attaque vous paralyse\n", creature->name);

    plongeur->effect = PARALYSED;
    is_paralysed = 1;
}
