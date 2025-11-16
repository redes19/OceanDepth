#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "../creature/creature.h"
#include "../joueur/joueur.h"
#include "combat.h"
#include "../clear/clear.h"
#include "../inventaire/inventaire.h"


int actionPlayer(Plongeur *plongeur, int choice, int depth, CreatureMarine *creature);

// =====================================
// Affiche un message et attend que l'utilisateur appuie sur Entrée.
//   Utilisé pour faire des pauses entre les étapes d'affichage.
// =====================================
void pressEnterToContinue()
{
    printf("\nAppuyez sur Entrée pour continuer...\n");
    getchar();
    getchar();
}

// =====================================
// Recherche dans le tableau global `tabOfCreature` une créature par son id.
//   Retourne un pointeur vers la `CreatureMarine` ou NULL si introuvable.
// =====================================
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

// =====================================
// Calcule un "niveau de profondeur" (entier) à partir d'une profondeur en mètres.
//   Valeurs retournées : 1,2,3,4 (utilisé pour coûts en oxygène).
// =====================================
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

// =====================================
// Attaque une créature identifiée par `idCreature` avec les stats du plongeur.
// =====================================
int AttackPlayer(Plongeur *plongeur, int idCreature, int depth)
{
    float damage;
    CreatureMarine *creature = findCreatureById(idCreature);
    if (!creature)
    {
        printf("Erreur : créature avec id %d introuvable.\n", idCreature);
        return -1;
    }

    float addProtection = (creature->effect == PROTECTED) ? 0.20f : 0.00f;
    float effectifProtection = creature->defense + addProtection;

    // plafonner protection pour pas soigner l'ennemie
    if (effectifProtection > 0.95f) {
        effectifProtection = 0.95f;
    }

    damage = plongeur->attack * (1.0f - effectifProtection);
    
    int damageFinal = (int)ceilf(damage);

    if (creature->life - damageFinal <= 0)
    {
        creature->life = 0;
    }

    // plongeur->niveau_fatigue++;
    plongeur->niveau_fatigue += 5;
    plongeur->niveau_oxygene -= depthLvl(depth);

    return creature->life -= damageFinal;
}

// =====================================
// Choix d'item utilisé par le plongeur
// =====================================
int ChoiceItem(Plongeur *plongeur) {
    int slot;
    printf("Indice du slot à utiliser : ");
    if (scanf("%d", &slot) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Entrée invalide.\n");
        return 1;
    }

    int res = inv_utiliser_objet(&plongeur->inv, slot-1,
        &plongeur->points_de_vie, plongeur->points_de_vie_max,
        &plongeur->niveau_oxygene, plongeur->niveau_oxygene_max,
        &plongeur->niveau_fatigue);

    if (res == 0) {
        printf("Objet utilisé avec succès.\n");
    } else {
        printf("Impossible d'utiliser cet objet (slot invalide / type non consommable / déjà full).\n");
    }
    return 1;
}

// =====================================
// Affiche l'inventaire du plongeur
// =====================================
void DisplayInventary(Plongeur *plongeur)
{
    printf("Choissi votre inventaire : \n");
    if(!displayInventaire(&plongeur->inv)) {
        return;
    }
    ChoiceItem(plongeur); 
    pressEnterToContinue();
}

// =====================================
// Fonction pour choisir la compétence que le plongeur a choisi
// =====================================
void choiceCompAquatique(Plongeur *plongeur, CreatureMarine *creature) {
    int choice;
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
        plongeur->comp[0](plongeur, creature);
        break;
     case 2:
        plongeur->comp[1](plongeur, creature);
        break;
     case 3:
        plongeur->comp[2](plongeur, creature);
        break;
    
    default:
        break;
    }
}

// =====================================
// Affiche les compétence du joueur
// =====================================
void AttackSpecialPlayer(Plongeur *plongeur, int depth, CreatureMarine *creature)
{
    displayCompAquatique();
    printf("Attack spezcial\n");
    choiceCompAquatique(plongeur, creature);
    plongeur->niveau_fatigue += 3;
    plongeur->niveau_oxygene -= 2 * depthLvl(depth);
}

// =====================================
// Diminue la fatigue du joueur
// =====================================
void DiminutionFatigue(Plongeur *plongeur, int choice, int dept, CreatureMarine *creature) {
    printf("Vous recuperez de l energie\n");
    if(plongeur->niveau_fatigue > 0) {
        plongeur->niveau_fatigue--;
    } else {
        printf("Vous avez aucune fatigue\n");
        actionPlayer(plongeur, choice, dept, creature);
    }
}

// =====================================
// Fonction pour choisir la creature à attaquer
// =====================================
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

// =====================================
// Fonction pour choisir l'action du plongeur
// =====================================
int actionPlayer(Plongeur *plongeur, int choice, int depth, CreatureMarine *creature)
{
    printf("choix : ");
    if (scanf("%d", &choice) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Entrée invalide.\n");
        return 1;
    }

    switch (choice)
    {
    case 1:
        if(plongeur->niveau_fatigue >= 5) {
            printf("Vous avez trop de fatigue pour attaquer!\n");
            actionPlayer(plongeur, choice, depth, creature);
        } else {
            AttackPlayer(plongeur, choiceCreature(), depth);
        }
        return 1;
    case 2:
        DisplayInventary(plongeur);
        return 0;
    case 3:
        if(plongeur->niveau_fatigue > 3) {
            printf("Vous avez pas assez d energie pour lance une attaque special\n");
            actionPlayer(plongeur, choice, depth, creature);
        } else {
            AttackSpecialPlayer(plongeur, depth, creature);
        }
        return 1;
    case 4 : 
        DiminutionFatigue(plongeur, choice, depth, creature);
        return 0;
    case 5 :
        return 0;
    default:
        printf("Veuillez choisir un choix valide!\n");
        printf("\033[2J\033[1;1H");
        initFight(plongeur, depth);
        return 1;
    }
}

// =====================================
// Affiche les choix d'action possible
// =====================================
int ChoicePlayer(int choice, Plongeur *plongeur, int depth, CreatureMarine *creature)
{
    printPlongeur(plongeur);
    printf("Vous avez %d de fatigue\n", plongeur->niveau_fatigue);

    printf("\n");
    printf("Que voulez-vous faire?\n");
    printf("1) Attaquer\n");
    printf("2) Utiliser son inventaire\n");
    printf("3) Attaque special\n");
    printf("4) Diminuez votre fatigue\n");
    printf("5) Mettre fin a votre tour\n");
    printf("\n");

    if(!actionPlayer(plongeur, choice, depth, creature)) {
        return 0;
    }

    return 1;
}

// =====================================
// Fonction d'attaque des creatures
// =====================================
int AttackCreature(Plongeur *plongeur, CreatureMarine *creature) 
{
    printf("La creature %s vous attaque et vous subissez %d de degats!\nVous perdez aussi de l'oxygene a cause du stress de l'attaque\n\n", creature->name, creature->max_attack);
    plongeur->niveau_oxygene -= 3;

    float damage = creature->max_attack * (1 - plongeur->defense);
    int dommageFinal = (int)ceilf(damage);

    return plongeur->points_de_vie -= dommageFinal;
}

// =====================================
// Fonction pour choisir l'action de la creature
// =====================================
void choiceActionCreature(CreatureMarine *creature, Plongeur *plongeur) {
    int choice = rand() % 4;

    if (!creature) {
        printf("Erreur: creature == NULL dans choiceActionCreature()\n");
        return;
    }

    if(choice == 1 && creature->effectCreature != NULL) {
        printf("%s lance une attaque special\n", creature->name);
        creature->effectCreature(creature, plongeur);
        printf("Vous perdez de l'oxygène du au stress de l'attaque\n");
        plongeur->niveau_oxygene -= 6;

    } else {
        AttackCreature(plongeur, creature);
    }

}

// =====================================
// Fonction de comparaison utilisée par qsort pour l'initiative.
// Compare la vitesse des entités (plongeur vs créature).
// =====================================
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

// =====================================
// Initialise le tableau d'initiative (Entité joueur + créatures) puis trié.
// Retourne un tableau d'Entity alloué
// =====================================
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

// =====================================
// Affiche les creatures 
// =====================================
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

// =====================================
// Vérifie si la vie du plongeur > 0
// =====================================
int playerIsAlive(Plongeur *plongeur)
{
    if (plongeur->points_de_vie <= 0)
    {
        printf("Vous avez perdu!!\n");
        return 0;
    }
    return 1;
}

// =====================================
// Vérifie l'oxygène du plongeur
// =====================================
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

// =====================================
// Vérifie s'il reste des creatures a combatre
// =====================================
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

// =====================================
// Vérifie si le plongeur est affecter par des effets
// =====================================
int checkEffectPlongeur(Plongeur *plongeur) {
    switch (plongeur->effect)
    {
    case POISONED:
        if (is_poisoned < 2) {
            is_poisoned++;
            printf("Vous perder 10 PV du au poison\n");
            // mettre les dégats du poison !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        } 
        
        if(is_poisoned > 2) {
            printf("Le poison a disparu\n");
            plongeur->effect = NO_EFFECT;
        }

        return 1;
    case PARALYSED:
        if (is_paralysed) {
            printf("Vous etes paralyse vous pouvez pas effectuer d'action");
            is_paralysed = 0;
            plongeur->effect = NO_EFFECT;
            return 0;
        } else {
            printf("Vous etes plus affecter par la paralisie\n");
            return 1;
        }
    default:
        break;
    }
    return 1;
}

// =====================================
// Vérifie si creature[i] est affecter par des effets
// =====================================
int checkEffectCreature(CreatureMarine *creature) {
    switch (creature->effect)
    {
    case PROTECTED:
        if (is_protected >= 2) {
            creature->effect = NO_EFFECT;
        } else {
            is_protected++;
        }
        return 1;
    case PARALYSED:
        if(creature->effect == PARALYSED) {
            creature->effect = NO_EFFECT;
            return 0;
        } else {
            return 1;
        }
        
    
    default:
        break;
    }
    return 1;
}

// =====================================
// Supprime les créature dans TabOfCreature dont la vie <= 0
// =====================================
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

// =====================================
// Supprime les créature dans InitiativeCreature dont la vie <= 0
// =====================================
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

// =====================================
// Fonction du combat
// =====================================
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
                printf("\nTour de la creature %s\n", c->name);
                
                if(!checkEffectCreature(c)) {
                    break;
                }

                choiceActionCreature(c, plongeur);
                pressEnterToContinue();
            }
            else if (initiative[i].type == ENT_PLONGEUR)
            {
                while (plongeur->niveau_fatigue <= 5 && total != 1) {
                    checkO2Plongeur(plongeur);

                    printf("\nTour du joueur\n");
                    if(!checkEffectPlongeur(plongeur)) {
                        break;
                    }

                    if(!ChoicePlayer(choice, plongeur, depth, c)) {
                        break;
                    }

                    total = deleteInitiativeCreature(total, initiative);
                    initiative = realloc(initiative, sizeof(Entity) * total);
                }

                if(plongeur->niveau_fatigue > 0) {
                    printf("Vous perdez 1 point de fatigue\n");
                    plongeur->niveau_fatigue--;
                }

                if(plongeur->niveau_oxygene <= 92) {
                    plongeur->niveau_oxygene += 8;
                    printf("Vous récupèrer 8 d'oxygene\n");
                }  
                printPlongeur(plongeur);
            }
        }
        printf("Fin du premier tour\n");
        pressEnterToContinue();
        // trouver alternative pour linux/mac
        clearScreen();
    }

    printf("Fin du combat!\n");
    free(initiative);
}

