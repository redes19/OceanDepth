#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../creature/creature.h"
#include "../joueur/joueur.h"
#include "combat.h"
#include "../clear/clear.h"

void actionPlayer(Plongeur *plongeur, int choice, int depth);

void pressEnterToContinue()
{
    printf("\nAppuyez sur Entrée pour continuer...\n");
    getchar();
    getchar();
}

static CreatureMarine *findCreatureById(int id)
{
    for (int i = 0; i < nb_creatures; i++)
    {
        if (tabOfCreature[i].id == id)
        {
            return &tabOfCreature[i];
        }
    }
    return NULL;
}

int depthLvl(int depth) {
    if (depth < 50) {
        return 1;
    } else if (depth < 150) {
        return 2;
    } else if( depth < 300) {
        return 3;
    }
    return 4;
}

int AttackPlayer(Plongeur *plongeur, int idCreature, int depth)
{
    CreatureMarine *creature = findCreatureById(idCreature);
    if (!creature)
    {
        printf("Erreur : créature avec id %d introuvable.\n", idCreature);
        return -1;
    }

    float dommage = plongeur->attack * (1 - creature->defense);
    int dommageFinal = (int)ceilf(dommage);

    if (creature->life - dommageFinal <= 0)
    {
        creature->life = 0;
    }

    plongeur->niveau_fatigue += 5;
    plongeur->niveau_oxygene -= depthLvl(depth);

    return creature->life -= dommageFinal;
}

void DisplayInventary(Plongeur *plongeur)
{
    printf("Ouverture inventaire\n");
    plongeur->niveau_fatigue += 2;
}

void AttackSpecialPlayer(Plongeur *plongeur, int depth)
{
    printf("Attack spezcial\n");
    plongeur->niveau_fatigue += 3;
    plongeur->niveau_oxygene -= 2 * depthLvl(depth);
}

void DiminutionFatigue(Plongeur *plongeur, int choice, int dept) {
    printf("Vous recuperez de l energie\n");
    if(plongeur->niveau_fatigue > 0) {
        plongeur->niveau_fatigue--;
    } else {
        printf("Vous avez aucune fatigue\n");
        actionPlayer(plongeur, choice, dept);
    }
}

int choiceCreature()
{
    int choice;
    while (1)
    {
        printf("Choissiez une creature a attaquer (choissisez son id): ");
        if (scanf("%d", &choice) != 1) {
            /* vider stdin si entrée invalide */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrée invalide.\n");
            continue;
        }

        for (int i = 0; i < nb_creatures; i++)
        {
            if (choice == tabOfCreature[i].id)
            {
                if (tabOfCreature[i].life > 0) {
                    return choice;
                } else {
                    printf("Choissiez une créature encore vivante!!\n");
                    break; /* sortir du for pour re-demander */
                }
            }
        }

        printf("ID introuvable ou créature morte. Veuillez reessayer.\n");
    }

    return 0;
}

void actionPlayer(Plongeur *plongeur, int choice, int depth)
{
    printf("choix : ");
    if (scanf("%d", &choice) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Entrée invalide.\n");
        return;
    }

    switch (choice)
    {
    case 1:
        if(plongeur->niveau_fatigue >= 5) {
            printf("Vous avez trop de fatigue pour attaquer!\n");
            actionPlayer(plongeur, choice, depth);
        } else {
            AttackPlayer(plongeur, choiceCreature(), depth);
        }
        break;
    case 2:
        DisplayInventary(plongeur);
        break;
    case 3:
        if(plongeur->niveau_fatigue > 3) {
            printf("Vous avez pas assez d energie pour lance une attaque special\n");
            actionPlayer(plongeur, choice, depth);
        } else {
            AttackSpecialPlayer(plongeur, depth);
        }
        break;
    case 4 : 
        DiminutionFatigue(plongeur, choice, depth);
        // Mettre fin du tour du player!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        break;
    case 5 :
        // utiliser var fatigue + initiative du joueur d'arreter son tour!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        break;
    default:
        printf("Veuillez choisir un choix valide!\n");
        printf("\033[2J\033[1;1H");
        initFight(plongeur, depth);
        break;
    }
}

void ChoicePlayer(int choice, Plongeur *plongeur, int depth)
{
    printf("\n");
    printf("Que voulez-vous faire?\n");
    printf("1) Attaquer\n");
    printf("2) Utiliser son inventaire\n");
    printf("3) Attaque special\n");
    printf("4) Diminuez votre fatigue\n");
    printf("5) Mettre fin a votre tour\n");
    printf("\n");

    actionPlayer(plongeur, choice, depth);
}

int AttackCreature(Plongeur *plongeur, CreatureMarine *creature) 
{
    printf("La creature %s vous attaque et vous subissez %d de degats!\nVous perdez aussi de l'oxygene a cause du stress de l'attaque\n\n", creature->name, creature->max_attack);
    plongeur->niveau_oxygene -= 5;

    float damage = creature->max_attack * (1 - plongeur->defense);
    int dommageFinal = (int)ceilf(damage);

    return plongeur->points_de_vie -= dommageFinal;
}

int cmp(const void *a, const void *b)
{
    const Entity *ea = a;
    const Entity *eb = b;

    int v1 = 0, v2 = 0;

    if (ea->type == ENT_PLONGEUR)
    {
        v1 = ea->u.plongeur.vitesse;
    }
    else
    {
        CreatureMarine *creature = findCreatureById(ea->u.creature_id);
        v2 = creature ? creature->vitesse : 0;
    }

    if (eb->type == ENT_PLONGEUR)
    {
        v1 = eb->u.plongeur.vitesse;
    }
    else
    {
        CreatureMarine *creature = findCreatureById(eb->u.creature_id);
        v2 = creature ? creature->vitesse : 0;
    }

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
        tabInitiative[i + 1].u.creature_id = tabOfCreature[i].id;
    }

    qsort(tabInitiative, total, sizeof(Entity), cmp);

    return tabInitiative;
}

void displayCreatures(Entity *initiative, int total)
{
    printf("\n\n==========================================================================\n");
    for (int i = 0; i < total; i++)
    {
        if (initiative[i].type == ENT_CREATURE)
        {
            CreatureMarine *c = findCreatureById(initiative[i].u.creature_id);
            printf("        id : %d - %-20s", c->id, c->name);
        }
    }

    printf("\n");

    for (int i = 0; i < total; i++)
    {
        if (initiative[i].type == ENT_CREATURE)
        {
            CreatureMarine *c = findCreatureById(initiative[i].u.creature_id);
            printf("                 %d/%d PV            ", c->life, c->max_life);
        }
    }
    printf("\n==========================================================================\n\n");
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

void checkO2Plongeur(Plongeur *plongeur) { 
    if (plongeur->niveau_oxygene <= 10) {
        printf("\n=============================================\n|                                           |");
        printf("\n|ATTENTION VOUS AVEZ PLUS BEAUCOUP D OXYGNE |\n");
        printf("|                                           |\n=============================================");
    }

    if(plongeur->niveau_oxygene <= 0) {
        plongeur->points_de_vie -= 5;
        printf("Vous n'avez plus d'oxygèn vous perdez 5 points de vie!\n");
    }
}

// Verifie s'il reste des creatures a combattre
int checkCreature(int total)
{
    if (total == 1 || nb_creatures == 0)
    {
        printf("Vous avez tue tout les creature marine!\n");
        pressEnterToContinue();
        return 0;
    }
    return 1;
}

// Supprime une creature si sa vie = 0
void deleteCreatureInTabOfCreature()
{
    for (int i = 0; i < nb_creatures; i++)
    {
        if (tabOfCreature[i].life == 0)
        {
            for (int j = i; j < nb_creatures - 1; j++)
            {
                tabOfCreature[j] = tabOfCreature[j + 1];
            }
            nb_creatures--;
            i--; // afin de ne pas sauté d'elements a cause du decalage
        }
    }
}

// Supprime les creatures morte
int deleteInitiativeCreature(int total, Entity *initiative)
{
    for (int i = 0; i < total; i++)
    {
        CreatureMarine *c = findCreatureById(initiative[i].u.creature_id);
        if (initiative[i].type == ENT_CREATURE)
        {
            if (c->life <= 0)
            {
                printf("\n========================================================\n\n");
                printf("\nVous avez elimine la creature %s!!\n", c->name);
                printf("\n========================================================\n\n");

                for (int k = 0; k < nb_creatures; k++)
                {
                    if (tabOfCreature[k].id == c->id)
                    {
                        tabOfCreature[k].life = 0;
                    }
                }

                deleteCreatureInTabOfCreature();

                for (int j = i; j < total - 1; j++)
                {
                    initiative[j] = initiative[j + 1];
                }
                total--;
                i--;
            }
        }
    }
    pressEnterToContinue();
    return total;
}

void initFight(Plongeur *plongeur, int depth)
{
    clearScreen();
    int choice = 0;
    printf("========================================================\nVous venez de lancé un combat contre %d Creature Marine\n========================================================\n", nb_creatures);

    int total = 1 + nb_creatures;
    Entity *initiative = initInitiative(plongeur, total);
    if (!initiative)
        return;

    while (playerIsAlive(plongeur) && checkCreature(total))
    {
        displayCreatures(initiative, total);
        printPlongeur(plongeur);
        for (int i = 0; i < total; i++)
        {
            CreatureMarine *c = findCreatureById(initiative[i].u.creature_id);

            if (initiative[i].type == ENT_CREATURE)
            {
                printf("\nTour de la creature %d : %s de vitesse : %d\n", c->id + 1, c->name, c->vitesse);
                AttackCreature(plongeur, c);
                pressEnterToContinue();
            }
            else if (initiative[i].type == ENT_PLONGEUR)
            {
                while (plongeur->niveau_fatigue < 5 && total != 1) {
                    printf("\nTour du joueur\n");
                    ChoicePlayer(choice, plongeur, depth);
                    total = deleteInitiativeCreature(total, initiative);
                    initiative = realloc(initiative, sizeof(Entity) * total);
                    checkO2Plongeur(plongeur);
                }
                if(plongeur->niveau_fatigue > 0) {
                    plongeur->niveau_fatigue--;
                }
            }
        }
        // trouver alternative pour linux/mac
        clearScreen();
    }

    printf("Fin du combat!\n");
    free(initiative);
}

// Ordre des actions par tour
// Initiative d'attaque selon vitesse
// Vérification a faire su le player ordre : 
//          Actions du joueur jusqu'à épuisement fatigue
//          Consommation oxygène automatique (-2 à -5 selon profondeur)
//          Vérification critique : si oxygène ≤ 10, alerte obligatoire
//          Récupération fatigue (+1 niveau)
// Vérification fin : victoire, défaite ou continuation
