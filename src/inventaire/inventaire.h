#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// =========================
// Constantes & Types de base
// =========================

#define INV_TAILLE_MAX 8
#define NOM_TAILLE_MAX 48

typedef enum {
    OBJ_CAPSULE_O2 = 0,   // +O2
    OBJ_TROUSSE_SOIN,     // +PV
    OBJ_ARME,             // Harpon / Trident / etc.
    OBJ_COMBINAISON       // Bonus DEF, O2/tour, etc.
} TypeObjet;

typedef struct {
    int atk_min;
    int atk_max;
    int o2_par_attaque;     // coût en oxygène par attaque
    int ignore_defense;     // points de défense ignorés
} StatsArme;

typedef struct {
    int bonus_defense;      // +DEF
    int o2_par_tour;        // coût ou bonus lié à l’oxygène/tour (peut être 0)
} StatsCombinaison;

typedef struct {
    TypeObjet type;
    char nom[NOM_TAILLE_MAX];
    int quantite;           // empilable pour consommables, 1 pour équipements

    // Données spécifiques selon le type
    union {
        struct { int gain_o2; } capsule;     // OBJ_CAPSULE_O2
        struct { int gain_pv; } trousse;     // OBJ_TROUSSE_SOIN
        StatsArme arme;                      // OBJ_ARME
        StatsCombinaison combi;              // OBJ_COMBINAISON
    } data;
} Objet;

typedef struct {
    Objet slots[INV_TAILLE_MAX];
    int nb_objets;              // nombre de slots effectivement occupés
    int idx_arme_equipee;       // index dans slots[] ou -1 si aucune
    int idx_combi_equipee;      // index dans slots[] ou -1 si aucune
    int perles;                 // monnaie du joueur
} Inventaire;

// =========================
// API Inventaire
// =========================

// Init / Reset
void inv_init(Inventaire *inv);

// Ajout/suppression d’objets
// - Empile les consommables identiques (même type + même nom + même effet)
// - Retourne 0 si OK, -1 si inventaire plein
int inv_ajouter_objet(Inventaire *inv, const Objet *obj);
int inv_supprimer_slot(Inventaire *inv, int slot_index); // compacte le tableau

// Utilisation de consommables sur le joueur
// Renvoie 0 si OK, -1 si slot invalide / type non consommable / ressources déjà au max
int inv_utiliser_objet(Inventaire *inv, int slot_index,
                       int *pv, int pv_max,
                       int *o2, int o2_max,
                       int *fatigue); // peut être ignoré (NULL) si non utilisé

// Équipement
// Renvoie 0 si OK, -1 si slot invalide ou mauvais type
int inv_equiper_arme(Inventaire *inv, int slot_index);
int inv_equiper_combinaison(Inventaire *inv, int slot_index);

// Accès rapides (NULL si non équipé)
const Objet* inv_get_arme_equipee(const Inventaire *inv);
const Objet* inv_get_combinaison_equipee(const Inventaire *inv);

// Récompenses
void inv_ajouter_perles(Inventaire *inv, int montant);
// Ajoute N loots (équipements ou consommables). Retourne le nombre réellement ajoutés.
int inv_ajouter_loots(Inventaire *inv, const Objet *loots, int n);

// =========================
// Sauvegarde / Chargement (format texte simple)
// =========================
//
// Format humain lisible (inspiré du sujet) :
//
// OCEANDEPTHS_INV_V1
// PERLES:<perles>
// NB:<nb_objets>
// SLOT:i:TYPE:<type>:NOM:<nom>:QTE:<qte>[:... champs spécifiques ...]
// EQUIPE:ARME:<index_ou_-1>:COMBI:<index_ou_-1>
//
// Champs spécifiques :
//  - CAPSULE:GAIN_O2:<val>
//  - TROUSSE:GAIN_PV:<val>
//  - ARME:ATK_MIN:<a>:ATK_MAX:<b>:O2_ATK:<c>:IGN_DEF:<d>
//  - COMBI:DEF:<x>:O2_TOUR:<y>
//
// Exemple :
// SLOT:0:TYPE:CAPSULE:NOM:Capsule_O2:QTE:3:CAPSULE:GAIN_O2:40

int inv_sauvegarder_fichier(const Inventaire *inv, const char *chemin);
int inv_charger_fichier(Inventaire *inv, const char *chemin);

// =========================
// Helpers de construction d’objets
// =========================
Objet objet_capsule(const char *nom, int quantite, int gain_o2);
Objet objet_trousse(const char *nom, int quantite, int gain_pv);
Objet objet_arme(const char *nom, int atk_min, int atk_max, int o2_par_attaque, int ignore_defense);
Objet objet_combinaison(const char *nom, int bonus_defense, int o2_par_tour);

#ifdef __cplusplus
}
#endif

#endif // INVENTAIRE_H
