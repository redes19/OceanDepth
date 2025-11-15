#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H


#include "../inventaire/inventaire.h"
#include "../joueur/joueur.h"
#include "../creature/creature.h"


// =============================================================
// SAUVEGARDE – EN-TÊTE
// Format TEXTE lisible (pratique pour corriger/debug) + inventaire inclus
// via un petit fichier compagnon (save.inv) géré par l'API Inventaire.
// =============================================================


typedef struct {
    int profondeur; // ex: 0 (surface) / 20 / 50 ...
    char zone[64]; // ex: "Recif_Corallien"
} Position;


// Écrit l'état du jeu dans un fichier (save.txt) + l'inventaire (save.inv)
int save_game(const char *path, const Plongeur *p, const Position *pos,
const CreatureMarine *creatures, int nb);


// Recharge une partie depuis le fichier + son inventaire compagnon
int load_game(const char *path, Plongeur *p, Position *pos,
CreatureMarine *creatures, int *nb, int capacity);


#endif