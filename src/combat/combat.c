#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../creature/creature.h"
#include "combat.h"

void optionActionPlayer() {
    printf("Que voulez-vous faire?\n");
    printf("1) Attaquer\n");
    printf("");
}

void initFight() {
    printf("===================================\nVous venez de lancé un combat contre %d Creature Marine\n===================================", nb_creatures);
}
