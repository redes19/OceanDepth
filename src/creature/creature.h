#ifndef CREATURE_H
#define CREATURE_H

typedef struct CreatureMarine {
    int id;
    char *name;
    int life;
    int max_life;
    int min_attack;
    int max_attack;
    float defense; // la défense est un pourcentage qui réduira les dégats du joueur
    int vitesse;
    char *effect;
    int is_alive;

} CreatureMarine;



CreatureMarine *createCreature(int depth);

#endif