#ifndef INVENTAIRE_H
#define INVENTAIRE_H


#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


// =============================================================
// INVENTAIRE – EN-TÊTE
// Objectif : gérer un sac de 8 objets max (consommables + équipements)
// Types d'objets :
// - Capsules d'oxygène (remontent l'O2)
// - Trousse de soin (remontent les PV)
// - Armes (Harpon, Trident...)
// - Combinaisons (bonus de défense, gestion O2/tour)
//
// Règles clés :
// - Consommables empilables (quantité > 1)
// - Équipements non empilables (quantité forcée à 1)
// - 8 slots max (INV_TAILLE_MAX)
// - On peut « équiper » 1 arme + 1 combinaison
// =============================================================


#define INV_TAILLE_MAX 8 // taille maxi de l'inventaire
#define NOM_TAILLE_MAX 48 // taille maxi pour le nom d'un objet


// Type d'objet stocké dans l'inventaire
typedef enum {
OBJ_CAPSULE_O2 = 0, // consommable : +O2
OBJ_TROUSSE_SOIN, // consommable : +PV
OBJ_ARME, // équipement : modifie l'attaque
OBJ_COMBINAISON // équipement : modifie la défense / O2 par tour
} TypeObjet;


// Caractéristiques d'une ARME
typedef struct {
int atk_min; // dégâts minimum
int atk_max; // dégâts maximum
int o2_par_attaque; // coût en oxygène par attaque
int ignore_defense; // points de défense ignorés (0 = rien)
} StatsArme;


// Caractéristiques d'une COMBINAISON
typedef struct {
int bonus_defense; // +DEF fourni par la combi
int o2_par_tour; // coût/bonus d'O2 par tour (peut être 0)
} StatsCombinaison;


// Représentation d'un OBJET générique de l'inventaire
typedef struct {
TypeObjet type; // capsule / trousse / arme / combi
char nom[NOM_TAILLE_MAX]; // nom affiché (ex: "Capsule_O2")
int quantite; // empilable si consommable, 1 si équipement


// Données spécifiques selon le type
union {
struct { int gain_o2; } capsule; // pour OBJ_CAPSULE_O2
struct { int gain_pv; } trousse; // pour OBJ_TROUSSE_SOIN
StatsArme arme; // pour OBJ_ARME
StatsCombinaison combi; // pour OBJ_COMBINAISON
} data;
} Objet;


// Un INVENTAIRE, c'est un tableau fixe de 8 slots + 2 index d'équipement
typedef struct {
Objet slots[INV_TAILLE_MAX];
int nb_objets; // nb de slots réellement utilisés
int idx_arme_equipee; // index de l'arme équipée, -1 si aucune
int idx_combi_equipee; // index de la combi équipée, -1 si aucune
int perles; // monnaie du joueur
} Inventaire;


// ========================
// API Inventaire – Fonctions
// ========================

void displayInventaire(const Inventaire *inv);


#endif // INVENTAIRE_H