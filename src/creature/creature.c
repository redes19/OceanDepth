#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "creature.h"


CreatureMarine *tabOfCreature = NULL;
int nb_creatures = 0;
static int random_initialized = 0;

// Fonction pour initialiser le générateur aléatoire une seule fois
void initRandom() {
    if (!random_initialized) {
        srand(time(NULL));
        random_initialized = 1;
    }
}

// Génération du nom des monstres aléatoirement
void NameMonster(CreatureMarine *creatureMarine) {

    const char *prefixes[] = {"Aqua", "Nero", "Mara", "Thalo", "Cora", "Hydro", "Medu"};
    const char *milieux[]  = {"li", "ra", "ne", "do", "so", "co"};
    const char *suffixes[] = {"th", "na", "ra", "os", "on", "te", "mi"};

    int nbrPre = sizeof(prefixes) / sizeof(prefixes[0]);
    int nbrMil = sizeof(milieux) / sizeof(milieux[0]);
    int nbrSu = sizeof(suffixes) / sizeof(suffixes[0]);

    const char *pre = prefixes[rand() % nbrPre];
    const char *mil = milieux[rand() % nbrMil];
    const char *su = suffixes[rand() % nbrSu];

    int nameSize = strlen(pre) + strlen(mil) + strlen(su) + 1;

    char *name = malloc(sizeof(char) * nameSize);
    if (name == NULL) {
        printf("Erreur d allocation pour le nom\n");
    }

    strcpy(name, pre);
    strcat(name, mil);
    strcat(name, su);

    creatureMarine->name = name;
}

// Ajout de stats au monstre selon leur nom
void addStatsCreature(CreatureMarine *creatureMarine) {
    const char *prefixes[] = {"Aqua", "Nero", "Mara", "Thalo", "Cora", "Hydro", "Medu"};

    const char *suffixes[] = {"th", "na", "ra", "os", "on", "te", "mi"};

    int nbrPrefixes = sizeof(prefixes) / sizeof(prefixes[0]);
    int nbrSuffixes = sizeof(suffixes) / sizeof(suffixes[0]);

    // ajout des stats selon le prefixe
    for (int i = 0; i < nbrPrefixes; i++) {
        if (strncmp(creatureMarine->name, prefixes[i], strlen(prefixes[i])) == 0) {
            if(strcmp(prefixes[i], "Medu") == 0 || strcmp(prefixes[i], "Cora") == 0 || strcmp(prefixes[i], "Thalo") == 0 ) {
                creatureMarine->max_life += rand() % (30 - 15 + 1) + 15;
            } else if (strcmp(prefixes[i], "Aqua") == 0 || strcmp(prefixes[i], "Hydro") == 0 ) {
                creatureMarine->max_attack += rand() % (20 - 5 + 1) + 5;
            } else if (strcmp(prefixes[i], "Nero") == 0 || strcmp(prefixes[i], "Mara") == 0 ) {
                creatureMarine->vitesse += rand() % (2 - 1 + 1) + 1;
            }
            
            if(strcmp(prefixes[i], "Nero") == 0 || strcmp(prefixes[i], "Medu") == 0) {
                creatureMarine->effectCreature = poisonBite;
            } else if(strcmp(prefixes[i], "Mara") == 0) {
                creatureMarine->effectCreature = frenesieSanguinaire;
            } else if(strcmp(prefixes[i], "Cora") == 0) {
                creatureMarine->effectCreature = carapace;
            } else if(strcmp(prefixes[i], "Aqua") == 0 || strcmp(prefixes[i], "Hydro") == 0) {
                creatureMarine->effectCreature = confusion;
            } else if(strcmp(prefixes[i], "Thalo") == 0) {
                creatureMarine->effectCreature = chargePerforante;
            }
        }
    }

    // ajout des stats selon le suffixe
    int nameLen = strlen(creatureMarine->name);
    int indexSufixe = nameLen - 2; // 2 car tout les suffixe sont de taille 2 (a changer plus tard)

    for (int i = 0; i < nbrSuffixes; i++) {
        if(strcmp(creatureMarine->name + indexSufixe, suffixes[i]) == 0) {
            if (strcmp(suffixes[i], "os") == 0 || strcmp(suffixes[i], "mi") == 0) {
                creatureMarine->vitesse += rand() % (2 - 1 + 1) + 1;
            } else if (strcmp(suffixes[i], "ra") == 0 || strcmp(suffixes[i], "th") == 0) {
                creatureMarine->defense += (rand() % 21 + 10) / 100.0f;
            } else if (strcmp(suffixes[i], "os") == 0) {
                creatureMarine->max_attack += rand() % (15 - 5 + 1) + 5;
            }
        }
    }

}

// initialise les stats poura augmenter la difficulté des monstres selon la profondeur
void DifficultyOfMonster(int depth, CreatureMarine *creatureMarine) {
    if ( depth >= 50 && depth < 150) {
        printf("depth entre 50 et 150\n");
        creatureMarine->max_life += 25;
        creatureMarine->vitesse += 1;
        creatureMarine->max_attack += 13;
    } else if (depth >= 150 && depth < 300) {
        printf("depth entre 150 et 300\n");
        creatureMarine->max_life += 60;
        creatureMarine->vitesse += 2;
        creatureMarine->max_attack += 28;
    } else if (depth >= 300) {
        printf("depth audessus de 300\n");
        creatureMarine->max_life += 100;
        creatureMarine->vitesse += 3;
        creatureMarine->max_attack += 34;
    }
}

// fonction pour générer un monstre
CreatureMarine *createCreature(int depth) {
    // printf("\nCreate Creature\n");

    initRandom();

    CreatureMarine *creatureMarine = malloc(sizeof(CreatureMarine));
    
    NameMonster(creatureMarine);

    // default params
    creatureMarine->is_alive = 1;
    creatureMarine->max_life = rand() % (35 - 25 + 1) + 25;
    creatureMarine->defense = 0;
    creatureMarine->max_attack = rand() % (12 - 8 + 1) + 8;
    creatureMarine->min_attack = creatureMarine->max_attack;
    creatureMarine->vitesse = 1;
    creatureMarine->effect = NO_EFFECT;
    creatureMarine->effectCreature = NULL;

    // fonction pour ajouter des stats aux créatures
    addStatsCreature(creatureMarine);
    DifficultyOfMonster(depth, creatureMarine);

    creatureMarine->life = creatureMarine->max_life;

    return creatureMarine;
}

int generateCreatureInTab(int n, int depth) {
    // printf("Generate Creature\n");

    if (n <= 0) {
        fprintf(stderr, "Erreur nombre de creature < a 1");
        return -1;
    }

    tabOfCreature = malloc(sizeof(CreatureMarine) * n);

    if (!tabOfCreature) {
        fprintf(stderr, "Erreur dans l'allocation de memoire du tableau");
        return -1;
    }

    nb_creatures = 0;

    for (int i = 0; i < n; i++) {
        CreatureMarine *newCreature = createCreature(depth);
        if (!newCreature) {
            fprintf(stderr, "Erreur dans l'allocation de memoire du tableau");
            for (int j = 0; j < nb_creatures; j++) {
                if (tabOfCreature[j].name) {
                    free(tabOfCreature[j].name);
                }
            }
            free(tabOfCreature);
            tabOfCreature = NULL;
            nb_creatures = 0;
            return -1;
        }
        
        tabOfCreature[nb_creatures] = *newCreature;
        tabOfCreature[nb_creatures].id = i;
        nb_creatures++;
        free(newCreature);

    }

    if (nb_creatures == 0) {
        fprintf(stderr, "Erreur aucune crature a pu etre creer");
        return -1;
    }

    // printf("\n %d creatures créer avec succès \n", nb_creatures);
    return 0;
}

void displayCreature() {
    printf("Display Creature\n");

    if (!tabOfCreature || nb_creatures == 0) {
        fprintf(stderr, "Pas de creature generer");
        return;
    }


    for (int i = 0; i < nb_creatures; i++) {
        printf("\n----------------------------------\n");
        printf("\nCreature : %d", tabOfCreature[i].id);
        printf("\nName : %s", tabOfCreature[i].name);
        printf("\nVie : %d", tabOfCreature[i].max_life);
        printf("\nDefense : %0.2f", tabOfCreature[i].defense);
        printf("\nVitesse : %d", tabOfCreature[i].vitesse);
        printf("\n---------------------------------\n");       
    }

    return;
}

// Fonction pour vider le tableau
void cleanupAllCreatures() {
    // printf("Nettoyage complet des créatures...\n");

    // Libération du tableau global s’il existe
    if (tabOfCreature != NULL) {
        for (int i = 0; i < nb_creatures; i++) {
            if (tabOfCreature[i].name != NULL) {
                free(tabOfCreature[i].name);
                tabOfCreature[i].name = NULL;
            }
        }

        free(tabOfCreature);
        tabOfCreature = NULL;
        nb_creatures = 0;

        // printf("OK Tableau de créatures libéré.\n");
    }

    // printf("Nettoyage mémoire terminé.\n");
}