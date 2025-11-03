#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "recompenses.h"


// =============================================================
// RÉCOMPENSES – IMPLÉMENTATION
// Utilise rand() – penser à appeler srand(time(NULL)) au lancement du jeu !
// =============================================================


// renvoie un entier aléatoire entre a et b (inclus)
static int rand_range(int a, int b){ return a + (rand() % (b - a + 1)); }


int distribuer_recompenses_creature(Plongeur *p, const CreatureMarine *c)
{
    if (!p || !c) return 0;


    // 1) Perles (toujours)
    // Idée simple : pourrait dépendre de la puissance de la créature
    int perles = rand_range(5, 15);
    inv_ajouter_perles(&p->inv, perles);


    // 2) Loots (consommables fréquents + 1 chance d'équipement)
    Objet drops[3];
    int n = 0;


    // Capsule O2 (fréquent)
    if (rand_range(1, 100) <= 60) {
        drops[n++] = objet_capsule("Capsule_O2", 1, 40); // +40 O2
    }
    // Trousse de soin (fréquent)
    if (rand_range(1, 100) <= 50) {
        drops[n++] = objet_trousse("Trousse_Soin", 1, 25); // +25 PV
    }
    // Équipement (rare)
    if (rand_range(1, 100) <= 20) {
        if (rand_range(0,1)==0) {
            drops[n++] = objet_arme("Harpon_Rouille", 12, 18, 2, 0);
        } else {
            drops[n++] = objet_combinaison("Neoprene_Basic", 5, 1);
        }
    }


    // On essaie d'ajouter tout ça dans l'inventaire
    int ajout = inv_ajouter_loots(&p->inv, drops, n);


    printf("\nRécompenses: +%d perles, %d objet(s) ajouté(s) à l’inventaire.\n", perles, ajout);
    return ajout;
}


void recompense_fin_combat(Plongeur *p, int nb_creatures_tuees)
{
    if (!p) return;
    // Bonus doux de fin de combat : facile à montrer en démo
    inv_ajouter_perles(&p->inv, nb_creatures_tuees * 2);
}
