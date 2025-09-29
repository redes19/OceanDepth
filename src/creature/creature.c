#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "creature.h"

// Génération du nom des monstres aléatoirement
void NameMonster(CreatureMarine *creatureMarine) {

    srand(time(NULL));

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
    srand(time(NULL));
    const char *prefixes[] = {"Aqua", "Nero", "Mara", "Thalo", "Cora", "Hydro", "Medu"};

    const char *suffixes[] = {"th", "na", "ra", "os", "on", "te", "mi"};

    int nbrPrefixes = sizeof(prefixes) / sizeof(prefixes[0]);
    int nbrSuffixes = sizeof(suffixes) / sizeof(suffixes[0]);

    // ajout des stats selon le prefixe
    for (int i = 0; i < nbrPrefixes; i++) {
        if (strncmp(creatureMarine->name, prefixes[i], strlen(prefixes[i])) == 0) {
            if(strcmp(prefixes[i], "Medu") == 0 || strcmp(prefixes[i], "Cora") == 0 || strcmp(prefixes[i], "Thalo") == 0 ) {
                printf("Life\n");
                creatureMarine->max_life += rand() % (45 - 25 + 1) + 25;
            } else if (strcmp(prefixes[i], "Aqua") == 0 || strcmp(prefixes[i], "Hydro") == 0 ) {
                printf("Attack\n");
                creatureMarine->max_attack += rand() % (20 - 5 + 1) + 5;
            } else if (strcmp(prefixes[i], "Nero") == 0 || strcmp(prefixes[i], "Mara") == 0 ) {
                printf("Speed\n");
                creatureMarine->vitesse += rand() % (2 - 1 + 1) + 1;
            }
        }
    }

    // ajout des stats selon le suffixe
    int nameLen = strlen(creatureMarine->name);
    int indexSufixe = nameLen - 2; // 2 car tout les suffixe sont de taille 2 (a changer plus tard)

    for (int i = 0; i < nbrSuffixes; i++) {
        if(strcmp(creatureMarine->name + indexSufixe, suffixes[i]) == 0) {
            if (strcmp(suffixes[i], "os") == 0 || strcmp(suffixes[i], "mi") == 0) {
                printf("Vitess\n");
                creatureMarine->vitesse += rand() % (2 - 1 + 1) + 1;
            } else if (strcmp(suffixes[i], "ra") == 0 || strcmp(suffixes[i], "th") == 0) {
                printf("Dfense\n");
                creatureMarine->defense += (rand() % 21 + 10) / 100.0f;
            } else if (strcmp(suffixes[i], "os") == 0) {
                printf("attack\n");
                creatureMarine->max_attack += rand() % (15 - 5 + 1) + 5;
            } else if (strcmp(suffixes[i], "on") == 0 || strcmp(suffixes[i], "te") == 0) {
                printf("Effect\n");
                // add effect
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
    srand(time(NULL));

    CreatureMarine *creatureMarine = malloc(sizeof(CreatureMarine));
    
    NameMonster(creatureMarine);

    // default params
    creatureMarine->is_alive = 1;
    creatureMarine->max_life = rand() % (35 - 25 + 1) + 25;
    creatureMarine->defense = 0;
    creatureMarine->max_attack = rand() % (12 - 8 + 1) + 8;
    creatureMarine->vitesse = 1;

    // fonction pour ajouter des stats aux créatures
    addStatsCreature(creatureMarine);
    DifficultyOfMonster(depth, creatureMarine);

    printf("depth %d\n", depth);

    creatureMarine->life = creatureMarine->max_life;

    printf("Name : %s life : %d attack : %d vitess : %d defense : %.2f\n", creatureMarine->name, creatureMarine->max_life, creatureMarine->max_attack, creatureMarine->vitesse, creatureMarine->defense);
    

    return creatureMarine;
}

// fonction qui définira combien de monstre devra être générer et seront enregister dans un tableau