#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../creature/creature.h"
#include "../joueur/joueur.h"
#include "combat.h"

int ActionPlayer() {
    printf("Que voulez-vous faire?\n");
    printf("1) Attaquer\n");
    printf("2) Recuperer de l'oxigene\n");
    printf("3) Utiliser son inventaire\n");
    printf("4) Attaque special\n");
    return 0;
}

int actionMonster() {
    return 0;
}

int cmp(const void *a, const void *b) {
        const Entity *ea = a;
        const Entity *eb = b;

        int v1 = (ea->type == ENT_PLONGEUR) ? ea->u.plongeur.vitesse : ea->u.creature.vitesse;
        int v2 = (eb->type == ENT_PLONGEUR) ? eb->u.plongeur.vitesse : eb->u.creature.vitesse;
        return v2-v1;
    }

Entity *initInitiative(Plongeur *plongeur, int total) {
    Entity *tabInitiative = malloc(sizeof(Entity) * total);
    if (!tabInitiative) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour tabInitiative");
        return NULL;
    }

    tabInitiative[0].type = ENT_PLONGEUR;
    tabInitiative[0].u.plongeur = *plongeur; 

    for (int i = 0; i < nb_creatures; i++) {
        tabInitiative[i+1].type = ENT_CREATURE;
        tabInitiative[i+1].u.creature = tabOfCreature[i];
    }

    qsort(tabInitiative, total, sizeof(Entity), cmp);

    return tabInitiative;
}

void initFight(Plongeur *plongeur) {
    printf("========================================================\nVous venez de lancé un combat contre %d Creature Marine\n========================================================\n", nb_creatures);

    int total = 1 + nb_creatures;
    Entity *initiative = initInitiative(plongeur, total);
    if (!initiative) return;

    printf("Ordre d'initiative (vitesse desc.) :\n");
    for (int i = 0; i < total; ++i) {
        if (initiative[i].type == ENT_PLONGEUR) {
            printf("%d) Plongeur (vitesse=%d)\n", i + 1, initiative[i].u.plongeur.vitesse);
        } else {
            printf("%d) Creature #%d %s (vitesse=%d)\n", i + 1,
                   initiative[i].u.creature.id,
                   initiative[i].u.creature.name ? initiative[i].u.creature.name : "??",
                   initiative[i].u.creature.vitesse);
        }
    }

    free(initiative);
}
