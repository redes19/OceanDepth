#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "creature.h"

void NameMonster(CreatureMarine *creatureMarine) {

    srand(time(NULL));

    const char *prefixes[] = {"Aqua", "Nero", "Mara", "Thalo", "Cora", "Hydro", "Medu"};
    const char *milieux[]  = {"li", "ra", "ne", "do", "so", "mi"};
    const char *suffixes[] = {"th", "na", "ra", "os", "on", "te"};

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
    
}

// fonction pour générer un monstre
CreatureMarine *createCreature(int depth) {
    CreatureMarine *creatureMarine = malloc(sizeof(CreatureMarine));
    
    NameMonster(creatureMarine);

    printf("%s\n", creatureMarine->name);

    // default params
    creatureMarine->is_alive = 1;
    creatureMarine->max_life = 30;
    creatureMarine->defense = 0;
    creatureMarine->max_attack = 8;
    creatureMarine->vitesse = 1;

    // fonction pour ajouter des stats aux créatures
    addStatsCreature(creatureMarine);
    

    return creatureMarine;
}

// fonction qui définira combien de monstre devra être générer et seront enregister dans un tableau