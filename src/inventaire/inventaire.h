#ifndef INVENTAIRE_H
#define INVENTAIRE_H


#include <stdio.h>

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

typedef struct Plongeur Plongeur;

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
void inv_init(Inventaire *inv);

int displayInventaire(const Inventaire *inv);

// Initialisation
void inv_init(Inventaire *inv);

// Gestion des objets
int inv_ajouter_objet(Inventaire *inv, const Objet *obj);
int inv_supprimer_slot(Inventaire *inv, int slot_index);
int inv_utiliser_objet(Inventaire *inv, int slot_index,
                       int *pv, int pv_max,
                       int *o2, int o2_max,
                       int *fatigue);

// Équipement
int inv_equiper_arme(Inventaire *inv, int slot_index);
int inv_equiper_combinaison(Inventaire *inv, int slot_index);
const Objet* inv_get_arme_equipee(const Inventaire *inv);
const Objet* inv_get_combinaison_equipee(const Inventaire *inv);

// Perles et récompenses
void inv_ajouter_perles(Inventaire *inv, int montant);
int inv_ajouter_loots(Inventaire *inv, const Objet *loots, int n);

// Sauvegarde/chargement
int inv_sauvegarder_fichier(const Inventaire *inv, const char *chemin);
int inv_charger_fichier(Inventaire *inv, const char *chemin);

// Helpers de construction d'objets
Objet objet_capsule(const char *nom, int quantite, int gain_o2);
Objet objet_trousse(const char *nom, int quantite, int gain_pv);
Objet objet_arme(const char *nom, int atk_min, int atk_max, int o2_par_attaque, int ignore_defense);
Objet objet_combinaison(const char *nom, int bonus_defense, int o2_par_tour);

#endif // INVENTAIRE_H