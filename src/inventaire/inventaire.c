#include "inventaire.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

// =============================================================
// INVENTAIRE – IMPLÉMENTATION
// Astuce : on commente TOUT ce qui n'est pas évident.
// =============================================================


// -------------------------------------------------------------
// Petite fonction utilitaire :
// Renvoie 1 si deux objets consommables sont « empilables » (compatibles).
// ≈ même type + même nom + mêmes valeurs de gain.
// Équipements (armes/combinaisons) NE SONT PAS empilables.
// -------------------------------------------------------------

static int objets_empilables_compatibles(const Objet *a, const Objet *b) {
    if (!a || !b) return 0;
    if (a->type != b->type) return 0;
    if (a->type == OBJ_ARME || a->type == OBJ_COMBINAISON) return 0; // non empilables
    if (strncmp(a->nom, b->nom, NOM_TAILLE_MAX) != 0) return 0;


    switch (a->type) {
        case OBJ_CAPSULE_O2:
            return (a->data.capsule.gain_o2 == b->data.capsule.gain_o2);
        case OBJ_TROUSSE_SOIN:
            return (a->data.trousse.gain_pv == b->data.trousse.gain_pv);
        default: return 0;
    }
}

// -------------------------------------------------------------
// Décale tous les slots vers la gauche à partir d'un index (suppression)
// + met à jour les index d'équipement si besoin.
// -------------------------------------------------------------
static void compacter_depuis(Inventaire *inv, int start) {
    for (int i = start; i < inv->nb_objets - 1; ++i) {
        inv->slots[i] = inv->slots[i + 1];
    }
    inv->nb_objets--;
    // Ajuster indices d’équipement (très important sinon crash/arme fantôme)
    if (inv->idx_arme_equipee == start) inv->idx_arme_equipee = -1;
    if (inv->idx_combi_equipee == start) inv->idx_combi_equipee = -1;
    if (inv->idx_arme_equipee > start) inv->idx_arme_equipee--;
    if (inv->idx_combi_equipee > start) inv->idx_combi_equipee--;
}

// -------------------------------------------------------------
// Supprime \n/\r en fin de chaîne (utile au chargement)
// -------------------------------------------------------------
static void tronquer_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n && (s[n-1] == '\n' || s[n-1] == '\r')) s[n-1] = '\0';
}

// =========================
// API publique
// =========================
void inv_init(Inventaire *inv) {
    if (!inv) return;
    memset(inv, 0, sizeof(*inv));
    inv->nb_objets = 0;
    inv->idx_arme_equipee = -1;
    inv->idx_combi_equipee = -1;
    inv->perles = 0;
}

// -------------------------------------------------------------
// Ajoute un objet dans l'inventaire.
// 1) si consommable compatible existe déjà → on empile (quantité++)
// 2) sinon, on ajoute dans un nouveau slot (si place disponible)
// -------------------------------------------------------------
int inv_ajouter_objet(Inventaire *inv, const Objet *obj) {
    if (!inv || !obj) return -1;


    // Tenter d'empiler sur un slot existant (consommables uniquement)
    for (int i = 0; i < inv->nb_objets; ++i) {
        if (objets_empilables_compatibles(&inv->slots[i], obj)) {
            long long sum = (long long)inv->slots[i].quantite + (long long)obj->quantite;
            // Sécurité : éviter l'overflow d'int
            inv->slots[i].quantite = (sum > 2147483647LL) ? 2147483647 : (int)sum;
            return 0; // empilé
        }
    }


    // Sinon on ouvre un nouveau slot
    if (inv->nb_objets >= INV_TAILLE_MAX) return -1; // plus de place
    inv->slots[inv->nb_objets] = *obj;


    // Important : équipements → quantité = 1 (non empilables)
    if (obj->type == OBJ_ARME || obj->type == OBJ_COMBINAISON) {
        inv->slots[inv->nb_objets].quantite = 1;
    }


    inv->nb_objets++;
    return 0;
}


int inv_supprimer_slot(Inventaire *inv, int slot_index) {
    if (!inv || slot_index < 0 || slot_index >= inv->nb_objets) return -1;
    compacter_depuis(inv, slot_index);
    return 0;
}

// -------------------------------------------------------------
// Utilise un consommable : Capsule O2 ou Trousse de soin.
// - Met à jour PV/O2 jusqu'au max
// - Décrémente la quantité et supprime le slot si 0
// -------------------------------------------------------------
int inv_utiliser_objet(Inventaire *inv, int slot_index,
int *pv, int pv_max,
int *o2, int o2_max,
int *fatigue)
{
    (void)fatigue; // non utilisé pour l'instant (mais prêt pour évoluer)


    if (!inv || slot_index < 0 || slot_index >= inv->nb_objets) return -1;
    Objet *o = &inv->slots[slot_index];


    // On refuse d'utiliser un équipement (pas consommable)
    if (o->type == OBJ_ARME || o->type == OBJ_COMBINAISON) return -1;


    switch (o->type) {
        case OBJ_CAPSULE_O2: {
            if (!o2) return -1; // sécurité
            if (*o2 >= o2_max) return -1; // déjà full O2
            int gain = o->data.capsule.gain_o2;
            *o2 += gain;
            if (*o2 > o2_max) *o2 = o2_max; // clamp
        } break;


        case OBJ_TROUSSE_SOIN: {
            if (!pv) return -1; // sécurité
            if (*pv >= pv_max) return -1; // déjà full PV
            int gain = o->data.trousse.gain_pv;
            *pv += gain;
            if (*pv > pv_max) *pv = pv_max; // clamp
        } break;


        default:
            return -1; // type inconnu
    }


    // On consomme l'objet (quantité -1) et on nettoie si vide
    if (o->quantite > 0) o->quantite--;
    if (o->quantite == 0) {
        inv_supprimer_slot(inv, slot_index);
    }
    return 0;
}

// -------------------------------------------------------------
// Équiper une arme / une combinaison (juste mémoriser l'index)
// -------------------------------------------------------------
int inv_equiper_arme(Inventaire *inv, int slot_index) {
    if (!inv || slot_index < 0 || slot_index >= inv->nb_objets) return -1;
    if (inv->slots[slot_index].type != OBJ_ARME) return -1;
    inv->idx_arme_equipee = slot_index;
    return 0;
}


int inv_equiper_combinaison(Inventaire *inv, int slot_index) {
    if (!inv || slot_index < 0 || slot_index >= inv->nb_objets) return -1;
    if (inv->slots[slot_index].type != OBJ_COMBINAISON) return -1;
    inv->idx_combi_equipee = slot_index;
    return 0;
}


const Objet* inv_get_arme_equipee(const Inventaire *inv) {
    if (!inv || inv->idx_arme_equipee < 0 || inv->idx_arme_equipee >= inv->nb_objets) return NULL;
    const Objet *o = &inv->slots[inv->idx_arme_equipee];
    return (o->type == OBJ_ARME) ? o : NULL;
}


const Objet* inv_get_combinaison_equipee(const Inventaire *inv) {
    if (!inv || inv->idx_combi_equipee < 0 || inv->idx_combi_equipee >= inv->nb_objets) return NULL;
    const Objet *o = &inv->slots[inv->idx_combi_equipee];
    return (o->type == OBJ_COMBINAISON) ? o : NULL;
}

// =========================
// Récompenses / Perles
// =========================
void inv_ajouter_perles(Inventaire *inv, int montant) {
    if (!inv) return;
    // Empêche de passer sous 0 (si montant négatif)
    if (montant < 0 && inv->perles + montant < 0) {
        inv->perles = 0;
    } else {
        inv->perles += montant;
    }
}


// Ajoute un petit tableau de loots (N objets) un par un.
int inv_ajouter_loots(Inventaire *inv, const Objet *loots, int n) {
    if (!inv || !loots || n <= 0) return 0;
    int ajoutes = 0;
    for (int i = 0; i < n; ++i) {
        if (inv_ajouter_objet(inv, &loots[i]) == 0) ajoutes++;
        // NOTE: si inventaire plein → on ignore silencieusement (simple pour un TP)
    }
    return ajoutes;
}

// =========================
// Sauvegarde / Chargement (format texte simple et lisible)
// =========================


// Fonctions utilitaires pour sérialiser/désérialiser
static const char *type_to_str(TypeObjet t) {
    switch (t) {
        case OBJ_CAPSULE_O2: return "CAPSULE";
        case OBJ_TROUSSE_SOIN:return "TROUSSE";
        case OBJ_ARME: return "ARME";
        case OBJ_COMBINAISON: return "COMBI";
        default: return "INCONNU";
    }
}


static TypeObjet str_to_type(const char *s) {
    if (!s) return OBJ_CAPSULE_O2; // valeur par défaut « gentille »
    if (strcmp(s, "CAPSULE") == 0) return OBJ_CAPSULE_O2;
    if (strcmp(s, "TROUSSE") == 0) return OBJ_TROUSSE_SOIN;
    if (strcmp(s, "ARME") == 0) return OBJ_ARME;
    if (strcmp(s, "COMBI") == 0) return OBJ_COMBINAISON;
    return OBJ_CAPSULE_O2;
}

int inv_sauvegarder_fichier(const Inventaire *inv, const char *chemin) {
    if (!inv || !chemin) return -1;
    FILE *f = fopen(chemin, "w");
    if (!f) return -1;


    fprintf(f, "OCEANDEPTHS_INV_V1\n");
    fprintf(f, "PERLES:%d\n", inv->perles);
    fprintf(f, "NB:%d\n", inv->nb_objets);


    // Un slot par ligne (lisible à la main, pratique pour un TP)
    for (int i = 0; i < inv->nb_objets; ++i) {
        const Objet *o = &inv->slots[i];
        fprintf(f, "SLOT:%d:TYPE:%s:NOM:%s:QTE:%d:",
        i, type_to_str(o->type), o->nom, o->quantite);


        // On ajoute les champs spécifiques selon le type
        switch (o->type) {
            case OBJ_CAPSULE_O2:
                fprintf(f, "CAPSULE:GAIN_O2:%d\n", o->data.capsule.gain_o2);
            break;
            case OBJ_TROUSSE_SOIN:
                fprintf(f, "TROUSSE:GAIN_PV:%d\n", o->data.trousse.gain_pv);
            break;
            case OBJ_ARME:
                fprintf(f, "ARME:ATK_MIN:%d:ATK_MAX:%d:O2_ATK:%d:IGN_DEF:%d\n",
                o->data.arme.atk_min, o->data.arme.atk_max,
                o->data.arme.o2_par_attaque, o->data.arme.ignore_defense);
            break;
            case OBJ_COMBINAISON:
                fprintf(f, "COMBI:DEF:%d:O2_TOUR:%d\n",
                o->data.combi.bonus_defense, o->data.combi.o2_par_tour);
            break;
            default:
                fprintf(f, "UNK\n");
            break;
        }
    }


    // Indique les index d'équipement (ou -1 si rien)
    fprintf(f, "EQUIPE:ARME:%d:COMBI:%d\n", inv->idx_arme_equipee, inv->idx_combi_equipee);


    fclose(f);
    return 0;
}

// Lecture d'une ligne + suppression du retour chariot
static int lire_ligne(FILE *f, char *buf, size_t n) {
    if (!f || !buf || n == 0) return 0;
    if (!fgets(buf, (int)n, f)) return 0;
    tronquer_newline(buf);
    return 1;
}

int inv_charger_fichier(Inventaire *inv, const char *chemin) {
    if (!inv || !chemin) return -1;
    FILE *f = fopen(chemin, "r");
    if (!f) return -1;


    char line[512];
    if (!lire_ligne(f, line, sizeof(line))) { fclose(f); return -1; }
    if (strcmp(line, "OCEANDEPTHS_INV_V1") != 0) { fclose(f); return -1; }


    inv_init(inv); // on part d'un inventaire vide


    // PERLES
    if (!lire_ligne(f, line, sizeof(line))) { fclose(f); return -1; }
    {
        int per = 0;
        if (sscanf(line, "PERLES:%d", &per) == 1) inv->perles = per;
    }


    // NB de slots à lire
    if (!lire_ligne(f, line, sizeof(line))) { fclose(f); return -1; }
    int nb = 0;
    if (sscanf(line, "NB:%d", &nb) != 1) { fclose(f); return -1; }

    // Lire chaque SLOT (avec strtok_r pour parser par ":")
    for (int s = 0; s < nb; ++s) {
        if (!lire_ligne(f, line, sizeof(line))) { fclose(f); return -1; }


        // Exemple de ligne :
        // SLOT:0:TYPE:CAPSULE:NOM:Capsule_O2:QTE:3:CAPSULE:GAIN_O2:40
        char *saveptr = NULL;
        char *tok = strtok_r(line, ":", &saveptr);
        if (!tok || strcmp(tok, "SLOT") != 0) { fclose(f); return -1; }


        tok = strtok_r(NULL, ":", &saveptr); // index (on s'en fiche)
        (void)tok;


        tok = strtok_r(NULL, ":", &saveptr); // "TYPE"
        if (!tok || strcmp(tok, "TYPE") != 0) { fclose(f); return -1; }


        tok = strtok_r(NULL, ":", &saveptr); // string du type
        if (!tok) { fclose(f); return -1; }
        TypeObjet t = str_to_type(tok);


        tok = strtok_r(NULL, ":", &saveptr); // "NOM"
        if (!tok || strcmp(tok, "NOM") != 0) { fclose(f); return -1; }


        tok = strtok_r(NULL, ":", &saveptr); // nom de l'objet
        if (!tok) { fclose(f); return -1; }
        char nom[NOM_TAILLE_MAX];
        strncpy(nom, tok, NOM_TAILLE_MAX - 1);
        nom[NOM_TAILLE_MAX - 1] = '\0';


        tok = strtok_r(NULL, ":", &saveptr); // "QTE"
        if (!tok || strcmp(tok, "QTE") != 0) { fclose(f); return -1; }


        tok = strtok_r(NULL, ":", &saveptr); // quantité
        if (!tok) { fclose(f); return -1; }
        int qte = atoi(tok);

        // On reconstitue l'objet
        Objet obj;
        memset(&obj, 0, sizeof(obj));
        obj.type = t;
        strncpy(obj.nom, nom, NOM_TAILLE_MAX - 1);
        obj.nom[NOM_TAILLE_MAX - 1] = '\0';
        obj.quantite = qte;


        // Champs spécifiques (optionnels selon le type)
        tok = strtok_r(NULL, ":", &saveptr); // TAG spécifique (peut être NULL)
        if (tok) {
            if (strcmp(tok, "CAPSULE") == 0) {
                tok = strtok_r(NULL, ":", &saveptr); // GAIN_O2
                tok = strtok_r(NULL, ":", &saveptr); // value
                obj.data.capsule.gain_o2 = tok ? atoi(tok) : 0;
            } else if (strcmp(tok, "TROUSSE") == 0) {
                tok = strtok_r(NULL, ":", &saveptr); // GAIN_PV
                tok = strtok_r(NULL, ":", &saveptr); // value
                obj.data.trousse.gain_pv = tok ? atoi(tok) : 0;
            } else if (strcmp(tok, "ARME") == 0) {
                // ARME:ATK_MIN:%d:ATK_MAX:%d:O2_ATK:%d:IGN_DEF:%d
                tok = strtok_r(NULL, ":", &saveptr); // ATK_MIN
                tok = strtok_r(NULL, ":", &saveptr); obj.data.arme.atk_min = tok ? atoi(tok) : 0;
                tok = strtok_r(NULL, ":", &saveptr); // ATK_MAX
                tok = strtok_r(NULL, ":", &saveptr); obj.data.arme.atk_max = tok ? atoi(tok) : 0;
                tok = strtok_r(NULL, ":", &saveptr); // O2_ATK
                tok = strtok_r(NULL, ":", &saveptr); obj.data.arme.o2_par_attaque = tok ? atoi(tok) : 0;
                tok = strtok_r(NULL, ":", &saveptr); // IGN_DEF
                tok = strtok_r(NULL, ":", &saveptr); obj.data.arme.ignore_defense = tok ? atoi(tok) : 0;
                obj.quantite = 1; // sécurité : une arme ne s'empile pas
            } else if (strcmp(tok, "COMBI") == 0) {
                // COMBI:DEF:%d:O2_TOUR:%d
                tok = strtok_r(NULL, ":", &saveptr); // DEF
                tok = strtok_r(NULL, ":", &saveptr); obj.data.combi.bonus_defense = tok ? atoi(tok) : 0;
                tok = strtok_r(NULL, ":", &saveptr); // O2_TOUR
                tok = strtok_r(NULL, ":", &saveptr); obj.data.combi.o2_par_tour = tok ? atoi(tok) : 0;
                obj.quantite = 1; // sécurité : une combi ne s'empile pas
            }
        }

        if (inv_ajouter_objet(inv, &obj) != 0) {
            // inventaire plein → on arrête proprement
            fclose(f);
            return -1;
        }
    }


    // Ligne EQUIPE : indices d'arme/combi (ou -1)
    if (!lire_ligne(f, line, sizeof(line))) { fclose(f); return -1; }
    // EQUIPE:ARME:%d:COMBI:%d
    int ia = -1, ic = -1;
    if (sscanf(line, "EQUIPE:ARME:%d:COMBI:%d", &ia, &ic) == 2) {
        inv->idx_arme_equipee = (ia >= 0 && ia < inv->nb_objets && inv->slots[ia].type == OBJ_ARME) ? ia : -1;
        inv->idx_combi_equipee = (ic >= 0 && ic < inv->nb_objets && inv->slots[ic].type == OBJ_COMBINAISON) ? ic : -1;
    }


    fclose(f);
    return 0;
}

// =========================
// Helpers de construction
// =========================


Objet objet_capsule(const char *nom, int quantite, int gain_o2) {
    Objet o; memset(&o, 0, sizeof(o));
    o.type = OBJ_CAPSULE_O2;
    strncpy(o.nom, nom ? nom : "Capsule O2", NOM_TAILLE_MAX - 1);
    o.quantite = (quantite > 0) ? quantite : 1;
    o.data.capsule.gain_o2 = gain_o2;
    return o;
}


Objet objet_trousse(const char *nom, int quantite, int gain_pv) {
    Objet o; memset(&o, 0, sizeof(o));
    o.type = OBJ_TROUSSE_SOIN;
    strncpy(o.nom, nom ? nom : "Trousse de Soin", NOM_TAILLE_MAX - 1);
    o.quantite = (quantite > 0) ? quantite : 1;
    o.data.trousse.gain_pv = gain_pv;
    return o;
}


Objet objet_arme(const char *nom, int atk_min, int atk_max, int o2_par_attaque, int ignore_defense) {
    Objet o; memset(&o, 0, sizeof(o));
    o.type = OBJ_ARME;
    strncpy(o.nom, nom ? nom : "Harpon", NOM_TAILLE_MAX - 1);
    o.quantite = 1;
    o.data.arme.atk_min = atk_min;
    o.data.arme.atk_max = atk_max;
    o.data.arme.o2_par_attaque = o2_par_attaque;
    o.data.arme.ignore_defense = ignore_defense;
    return o;
}


Objet objet_combinaison(const char *nom, int bonus_defense, int o2_par_tour) {
    Objet o; memset(&o, 0, sizeof(o));
    o.type = OBJ_COMBINAISON;
    strncpy(o.nom, nom ? nom : "Combinaison", NOM_TAILLE_MAX - 1);
    o.quantite = 1;
    o.data.combi.bonus_defense = bonus_defense;
    o.data.combi.o2_par_tour = o2_par_tour;
    return o;
}

/* Affiche une chaîne lisible pour le type */
static const char *type_to_label(TypeObjet t){
  switch (t){
    case OBJ_CAPSULE_O2: return "Capsule 02";
    case OBJ_TROUSSE_SOIN: return "Trousse de soin";
    case OBJ_ARME: return "Arme";
    case OBJ_COMBINAISON: return "Combison";
    default: return "Inconnu";
  }
}

/* Affiche l'inventaire passé en paramètre */
void displayInventaire(const Inventaire *inv){
  if (!inv) {
    printf("Iventaire invalide.\n");
    return;
  }

  printf("=== Iventaire ===\n");
  printf("Perles : %d\n", inv->perles);
  printf("Slots  utilisés: %d/%d\n\n", inv->nb_objets, INV_TAILLE_MAX);

  if(inv->nb_objets == 0){
    printf("L'inventaire est vide;\n");
    return;
  }

  for (int i =0; i < inv->nb_objets; i++){
    const Objet *o = &inv->slots[i];
    const char *label = type_to_label(o->type);
    printf("[%d] %s - %s (qte: %d)", i, label, o->nom, o->quantite);

    /* Marquer les équipements équipés */
    if(i== inv->idx_arme_equipee) printf(" <-- Arme équipéé");
    if(i== inv->idx_combi_equipee) printf(" <-- Combinaison équipéé");

    /* Détails selon le type */
    switch (o->type){
      case OBJ_CAPSULE_O2:
        printf(" | +%d 02\n", o->data.capsule.gain_o2);
        break;
      case OBJ_TROUSSE_SOIN:
        printf(" | +%d PV\n", o->data.trousse.gain_pv);
        break;
      case OBJ_ARME:
        printf(" | ATK: %d-%d | O2/par: %d | IgnoreDEF: %d\n",
          o->data.arme.atk_min,
          o->data.arme.atk_max,
          o->data.arme.o2_par_attaque,
          o->data.arme.ignore_defense);
        break;
      case OBJ_COMBINAISON:
        printf(" | +DEF:%d 02/tour: %d\n", o->data.combi.bonus_defense, o->data.combi.o2_par_tour);
        break;
      default:
        printf(" | (details inconnus)\n");
        break;
    }
  }
  printf("=================\n");
}

/* Wrapper "legacy" utilisé ailleurs dans le projet */
void DisplayIventaire(void){
  printf("Affichage inventaire indisponible ici : utilisez displayIventaire(&plongeur->inv)\n");
 }