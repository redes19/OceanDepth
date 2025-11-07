#include <stdio.h>

#include "joueur.h"
#include "../creature/creature.h"

void apneeProlongee(Plongeur *plongeur, void *target) {
    (void)target;
    printf("Vous recupere 20 d'oxygene\n");
    if(plongeur->niveau_oxygene > 80) {
        plongeur->niveau_oxygene = 100;
    } else {
        plongeur->niveau_oxygene += 20;
    }
}

void dechargeElectrique(Plongeur *plongeur, void *target) {
    CreatureMarine *creature = (CreatureMarine *)target;

    printf("Vous effectuer decharge electrique, ce qui paralyse les créatures\n");

    for(int i = 0; i< nb_creatures; i++) {
        creature->effect = PARALYSED;
        creature->life -= 20;
    }

}

void communicationMarine(Plongeur *plongeur, void *target) {
    CreatureMarine *creature = (CreatureMarine *)target;

    printf("Vous pacifiez %s prendant 1 tour, %s ne vous attaquera pas\n", creature->name);

    creature->effect = PARALYSED;
}