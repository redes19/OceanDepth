#ifndef CREATURE_H
#define CREATURE_H

typedef struct CreatureMarine {
    int id;
    char *name;
    int life;
    int max_life;
    int min_attack;
    int max_attack;
    int defense;
    int vitesse;
    char *effect;
    int is_alive;

} CreatureMarine;


#endif