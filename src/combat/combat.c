#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../creature/creature.h"
#include "../joueur/joueur.h"
#include "combat.h"

void pressEnterToContinue()
{
    printf("\nAppuyez sur Entrée pour continuer...\n");
    getchar();
    getchar();
}

int AttackPlayer(Plongeur *plongeur, CreatureMarine *creature) // refacto
{
    printf("Attack player\n");

    int dommage = plongeur->attack * (1 - creature->defense);
    int finalDommage = (int)ceilf(dommage);

    creature->life -= finalDommage;

    if (creature->life <= 0)
    {
        return creature->life = 0;
    }

    return creature->life;
}

void DisplayInventary()
{
    printf("Ouverture inventaire\n");
}

void AttackSpecialPlayer()
{
    printf("Attack spezcial\n");
}

int choiceCreature()
{
    int choice;
    printf("Choissiez une creature a attaquer (1 a %d): ", nb_creatures);
    scanf("%d", &choice);

    while (choice < 1 || choice > nb_creatures)
    {
        printf("Entree invalide!\n");
        printf("Choissiez une creature a attaquer (1-%d): ", nb_creatures);
        scanf("%d", &choice);
    }

    return choice - 1;
}

void actionPlayer(int choice, Plongeur *plongeur)
{
    printf("choix : ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        int num = choiceCreature();
        AttackPlayer(plongeur, &tabOfCreature[num]);
        break;
    case 2:
        DisplayInventary();
        break;
    case 3:
        AttackSpecialPlayer();
        break;
    default:
        printf("Veuillez choisir un choix valide!\n");
        system("cls");
        initFight(plongeur);
        break;
    }
}

void ChoicePlayer(int choice, Plongeur *plongeur)
{
    printf("\n");
    printf("Que voulez-vous faire?\n");
    printf("1) Attaquer\n");
    printf("2) Utiliser son inventaire\n");
    printf("3) Attaque special\n");
    printf("\n");

    actionPlayer(choice, plongeur);
}

int AttackCreature(Plongeur *plongeur, CreatureMarine *creature) // refacto
{
    printf("La creature %s vous attaque et vous subissez %d de degats!\n", creature->name, creature->max_attack);
    return plongeur->points_de_vie -= creature->max_attack;
}

int cmp(const void *a, const void *b)
{
    const Entity *ea = a;
    const Entity *eb = b;

    int v1 = (ea->type == ENT_PLONGEUR) ? ea->u.plongeur.vitesse : ea->u.creature.vitesse;
    int v2 = (eb->type == ENT_PLONGEUR) ? eb->u.plongeur.vitesse : eb->u.creature.vitesse;
    return v2 - v1;
}

Entity *initInitiative(Plongeur *plongeur, int total)
{
    Entity *tabInitiative = malloc(sizeof(Entity) * total);
    if (!tabInitiative)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire pour tabInitiative");
        return NULL;
    }

    tabInitiative[0].type = ENT_PLONGEUR;
    tabInitiative[0].u.plongeur = *plongeur;

    for (int i = 0; i < nb_creatures; i++)
    {
        tabInitiative[i + 1].type = ENT_CREATURE;
        tabInitiative[i + 1].u.creature = tabOfCreature[i];
    }

    qsort(tabInitiative, total, sizeof(Entity), cmp);

    return tabInitiative;
}

void displayCreatures(Entity *initiative, int total)
{
    printf("\n\n========================================================\n");
    for (int i = 0; i < total; i++)
    {
        if (initiative[i].type == ENT_CREATURE) {
            printf("        id : %d - %-20s",  initiative[i].u.creature.id+1, initiative[i].u.creature.name);
        }
    }

    printf("\n");

    for (int i = 0; i < total; i++)
    {
        if (initiative[i].type == ENT_CREATURE) {
            printf("                 %d/%d PV            ", initiative[i].u.creature.life, initiative[i].u.creature.max_life);
        }
    }
    printf("\n========================================================\n\n");
}

int playerIsAlive(Plongeur *plongeur)
{
    if (plongeur->points_de_vie <= 0)
    {
        printf("Vous avez perdu!!\n");
        return 0;
    }
    return 1;
}

// Verifie s'il reste des creatures a combattre
int checkCreature()
{
    if (nb_creatures == 0)
    {
        printf("Vous avez tue tout les creature marine!\n");
        return 0;
    }
    return 1;
}

// Supprime les creatures morte
void deleteInitiativeCreature(int total, Entity *initiative) // refacto
{
    for (int i = 0; i < total; i++)
    {
        if (initiative[i].type == ENT_CREATURE)
        {
            printf("name : %s - life : %d", initiative[i].u.creature.name, initiative[i].u.creature.life);
            if (initiative[i].u.creature.life == 0)
            {
                for (int j = i; j < total - 1; j++)
                {
                    initiative[j] = initiative[j + 1];
                }
                total--;
                i--;
            }
        }
    }
}

// Supprime une creature si sa vie = 0
void deleteCreatureInTabOfCreature(int total, Entity *initiative) // refacto
{
    for (int i = 0; i < nb_creatures; i++)
    {
        if (tabOfCreature[i].life == 0)
        {
            printf("\n========================================================\n\n");
            printf("\nVous avez elimine une creature!!\n");
            printf("\n========================================================\n\n");
            for (int j = i; j < nb_creatures - 1; j++)
            {
                tabOfCreature[j] = tabOfCreature[j + 1];
            }
            nb_creatures--;
            i--; // afin de ne pas sauté d'elements a cause du decalage
        }
    }
    deleteInitiativeCreature(total, initiative);
    pressEnterToContinue();
}

void initFight(Plongeur *plongeur)
{
    plongeur->attack = 100;
    int choice = 0;
    printf("========================================================\nVous venez de lancé un combat contre %d Creature Marine\n========================================================\n", nb_creatures);

    int total = 1 + nb_creatures;
    Entity *initiative = initInitiative(plongeur, total);
    if (!initiative)
        return;

    while (playerIsAlive(plongeur) && checkCreature())
    {
        displayCreatures(initiative, total);
        printPlongeur(plongeur);
        for (int i = 0; i < total; i++)
        {

            if (initiative[i].type == ENT_CREATURE)
            {
                if (initiative[i].u.creature.life == 0)
                    continue;
            }

            if (initiative[i].type == ENT_CREATURE)
            {
                printf("Tour de la creature %d : %s de vitesse : %d\n", initiative[i].u.creature.id + 1, initiative[i].u.creature.name, initiative[i].u.creature.vitesse);
                AttackCreature(plongeur, &initiative[i].u.creature);
                pressEnterToContinue();
            }
            else if (initiative[i].type == ENT_PLONGEUR)
            {
                printf("Tour du joueur\n");
                ChoicePlayer(choice, plongeur);
                deleteCreatureInTabOfCreature(total, initiative);
                displayCreatures(initiative, total);
            }
        }
    }

    printf("Fin du combat!\n");
    free(initiative);
}
