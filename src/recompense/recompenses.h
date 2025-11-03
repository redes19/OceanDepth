#ifndef RECOMPENSES_H
#define RECOMPENSES_H


#include "../inventaire/inventaire.h"
#include "../joueur/joueur.h" // définit Plongeur (PV/O2/Fatigue + Inventaire)
#include "../creature/creature.h" // définit CreatureMarine


// =============================================================
// RÉCOMPENSES – EN-TÊTE
// Idée : quand une créature meurt → on donne des perles + quelques loots.
// Taux (exemple simple, parfait pour un TP) :
// - Perles : 5 à 15 (toujours)
// - Capsule O2 : 60% de chance (gain O2 = 40)
// - Trousse : 50% de chance (gain PV = 25)
// - Équipement rare : 20% (Harpon rouillé OU Néoprène basic)
// =============================================================


// Distribue les récompenses d'une créature morte.
// Retourne le nb d'objets effectivement ajoutés à l'inventaire.
int distribuer_recompenses_creature(Plongeur *p, const CreatureMarine *c);


// Petit bonus de fin de combat (ex : +2 perles / créature tuée)
void recompense_fin_combat(Plongeur *p, int nb_creatures_tuees);


#endif
