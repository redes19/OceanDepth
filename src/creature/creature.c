#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "creature.h"

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
                creatureMarine->vitesse += rand() % (3 - 0 + 1) + 0;
            }
        }
    }

    // ajout des stats selon le suffixe

}

// fonction pour générer un monstre
CreatureMarine *createCreature() {
    CreatureMarine *creatureMarine = malloc(sizeof(CreatureMarine));
    
    NameMonster(creatureMarine);

    // default params
    creatureMarine->is_alive = 1;
    creatureMarine->max_life = 30;
    creatureMarine->defense = 0;
    creatureMarine->max_attack = 8;
    creatureMarine->vitesse = 1;

    // fonction pour ajouter des stats aux créatures
    addStatsCreature(creatureMarine);

    creatureMarine->life = creatureMarine->max_life;

    printf("Name : %s life : %d attack : %d vitess : %d defense : %.2f\n", creatureMarine->name, creatureMarine->max_life, creatureMarine->max_attack, creatureMarine->vitesse, creatureMarine->defense);
    

    return creatureMarine;
}

// fonction qui définira combien de monstre devra être générer et seront enregister dans un tableau