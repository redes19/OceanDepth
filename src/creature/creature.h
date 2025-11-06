#ifndef CREATURE_H
#define CREATURE_H

struct CreatureMarine;
typedef void(*EffectSpeCreature) (struct CreatureMarine *creatureMarine, void *target);

typedef enum {
    POISONED, // Retire 10PV 
    PARALYSED, // Saut le tour
    PROTECTED, // Réduit de 20% les dégats
    REDUCEDDAMAGE, // Réduction des dégats
    NO_EFFECT,
} Effect;

typedef struct CreatureMarine {
    int id;
    char *name;
    int life;
    int max_life;
    int min_attack;
    int max_attack;
    float defense; // la défense est un pourcentage qui réduira les dégats du joueur
    int vitesse;
    Effect effect;
    int is_alive;
    EffectSpeCreature effectCreature;

} CreatureMarine;

// Effect Spécials
void poisonBite(CreatureMarine *creatureMarine, void *target);
void frenesieSanguinaire(CreatureMarine *creatureMarine, void *target);
void carapace(CreatureMarine *creature, void *target);
void chargePerforante(CreatureMarine *creature, void *target);
void confusion(CreatureMarine *creature, void *target);


// Déclarations des variables globales
extern CreatureMarine *tabOfCreature;
extern int nb_creatures;

CreatureMarine *createCreature(int depth);

int generateCreatureInTab(int n, int depth);
void displayCreature();

// Fonction de free
void cleanupAllCreatures();

#endif