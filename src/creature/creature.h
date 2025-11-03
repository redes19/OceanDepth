#include <joueur.h>
#ifndef CREATURE_H
#define CREATURE_H

typedef void(*EffectSpeCreature) (struct CreatureMarine *creatureMarine, void *target);

typedef struct CreatureMarine {
    int id;
    char *name;
    int life;
    int max_life;
    int min_attack;
    int max_attack;
    float defense; // la défense est un pourcentage qui réduira les dégats du joueur
    int vitesse;
    char effect[20];
    int is_alive;
    EffectSpeCreature attackSpeCreature;

} CreatureMarine;

typedef enum {
    POISONED,
    PARALYSED,
} Effect;


// Déclarations des variables globales
extern CreatureMarine *tabOfCreature;
extern int nb_creatures;

CreatureMarine *createCreature(int depth);

int generateCreatureInTab(int n, int depth);
void displayCreature();

// Fonction de free
void cleanupAllCreatures();

#endif