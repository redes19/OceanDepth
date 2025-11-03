#include "creature.h"
#include "../joueur/joueur.h"

void poisonBite(CreatureMarine *creatureMarine, void *target) {
    Plongeur *plongeur = (Plongeur *)target;
    int damage = (creatureMarine->max_attack + creatureMarine->min_attack) / 2;
    printf("Vous subissez l'effet Speciale PoisonBite de %s\n", creatureMarine->name);
    printf("Vous perdez %d de point de vie!\n", damage);

    plongeur->points_de_vie -= damage;
    plongeur->effet = POISONED;
}

void frenesieSanguinaire(CreatureMarine *creatureMarine, void *target) {
    if (creatureMarine->life < 50 ) {
        creatureMarine->max_attack += (creatureMarine->max_attack * 30) / 100;
    }
}