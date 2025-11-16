#include "carte.h"
#include "../joueur/joueur.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Crée une nouvelle carte dynamique 4x4
Zone **newCarte() {
    int rows = 4;
    int cols = 4;

    Zone **carte = malloc(rows * sizeof(Zone*));


    for (int i = 0; i < rows; i++) {
        carte[i] = malloc(cols * sizeof(Zone));
    }

    carte[0][0] = (Zone){"Base", 0, 0, 0};
    carte[0][1] = (Zone){"Ocean", 0, 0, 0};
    carte[0][2] = (Zone){"Epave", 0, 1, 1};
    carte[0][3] = (Zone){"Grotte", 0, 1, 0};

    carte[1][0] = (Zone){"Recif", 50, 2, 0};
    carte[1][1] = (Zone){"Epave", 50, 2, 1};
    carte[1][2] = (Zone){"Algues", 50, 0, 0};
    carte[1][3] = (Zone){"Grotte", 50, 1, 0};

    carte[2][0] = (Zone){"Requin", 150, 3, 0};
    carte[2][1] = (Zone){"Vide", 150, 0, 0};
    carte[2][2] = (Zone){"Kraken", 150, 5, 1};
    carte[2][3] = (Zone){"Vide", 150, 0, 0};

    carte[3][0] = (Zone){"Abyss", 300, 4, 1};
    carte[3][1] = (Zone){"Fosse", 300, 2, 0};
    carte[3][2] = (Zone){"Epave Géante", 300, 6, 1};
    carte[3][3] = (Zone){"Caverne Noire", 300, 3, 0};

    return carte;
}

int a_la_carte(Inventaire *inv, int niveau_cible) {
    if (!inv) return 0;
    for (int i = 0; i < inv->nb_objets; i++) {
        Objet *o = &inv->slots[i];
        if (o->type == OBJ_CARTE && o->data.carte.niveau == niveau_cible && o->quantite > 0) {
            return 1; // carte trouvée
        }
    }
    return 0; // pas de carte
}


// Affiche toute la carte
void printCarte(Plongeur *plongeur, Zone **zone) {
    for (int i = 0; i < 4; i++) {
        int afficher = 0;

        // Vérifie si cette ligne contient au moins une case plus profonde
        for (int j = 0; j < 4; j++) {
            if (zone[i][j].profondeur <= plongeur->zone->profondeur) {
                afficher = 1;
                break;
            }
        }

        if (afficher) {
            printf("---- ZONE %d ----\n", i+1);
            for (int k = 0; k < 4; k++) {
                    printf("%s\n", zone[i][k].nom);
            }
            printf("\n");
        }else{
            printf("---- ZONE %d ----\n", i+1);
            for (int k = 0; k < 4; k++) {
                    printf("???\n");
            }
        }
    }

}

// Affiche les zones visibles pour le plongeur et demande une action
int printZone(Plongeur *plongeur, Zone **zone) {
    for (int i = 0; i < 4; i++) {
        // Vérifie si la profondeur correspond à celle du plongeur
        
        if (zone[i][0].profondeur == plongeur->zone->profondeur) {
            printf("---- ZONE %d ----\n", i+1);
            for (int j = 0; j < 4; j++) {                       
                printf("%s\n", zone[i][j].nom);                           
                printf("\n");
            }
        }
    }

    printf("Vous êtes dans la zone : %s\n", plongeur->zone->nom);
    printf("Quelle action voulez-vous effectuer (taper le chiffre)\n"
           "1 - Se deplacer\n"
           "2 - Explorer la zone\n"
           "3 - Retour surface\n"
           "4 - Carte detaillee\n");

    int action = 0;
    scanf("%d", &action);
    return action;
}

void deplacement(Plongeur *joueur, Zone **carte) {
    int ligne = -1, colonne = -1;

    // Trouver la position actuelle du joueur
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (&carte[i][j] == joueur->zone) {
                ligne = i;
                colonne = j;
                break;
            }
        }
    }

    if (ligne == -1 || colonne == -1) {
        printf("Erreur : position du joueur introuvable.\n");
        return;
    }

    printf("Vous êtes actuellement dans : %s (ligne %d, colonne %d)\n", 
           joueur->zone->nom, ligne + 1, colonne + 1);

    // Afficher options
    printf("1 - Aller à gauche\n");
    printf("2 - Aller à droite\n");

    // Monter seulement si on n'est pas en surface
    if (ligne > 0) {
        printf("3 - Monter\n");
    }
    // Descendre seulement si on a la carte pour le niveau inférieur
    if (ligne < 3) {
        printf("4 - Descendre (nécessite carte)\n");
    }

    int choix = 0;
    scanf("%d", &choix);

    int ligne_suivante = ligne;
    int colonne_suivante = colonne;

    switch (choix) {
        case 1: // gauche
            colonne_suivante = (colonne - 1 + 4) % 4;
            break;
        case 2: // droite
            colonne_suivante = (colonne + 1) % 4;
            break;
        case 3: // monter
            if (ligne == 0) {
                printf("Vous êtes déjà en surface, impossible de monter.\n");
                return;
            }
            ligne_suivante = ligne - 1;
            break;
        case 4: // descendre
            ligne_suivante = ligne + 1;
            if (!a_la_carte(&joueur->inv, ligne_suivante + 1)) {
                printf("Vous n'avez pas la carte pour descendre à ce niveau !\n");
                return;
            }
            break;
        default:
            printf("Choix invalide.\n");
            return;
    }

    joueur->zone = &carte[ligne_suivante][colonne_suivante];
    printf("Vous vous déplacez vers : %s\n", joueur->zone->nom);
}

#include <stdlib.h>
#include <time.h>

void explorerZone(Plongeur *joueur) {
    Zone *z = joueur->zone;

    // Si la zone est déjà vide
    if (z->tresor == 0 && z->ennemis == 0) {
        printf("Vous avez déjà exploré cette zone, rien à trouver.\n");
        return;
    }

    if (z->tresor > 0 && z->ennemis > 0) {
        // Les deux présents → tirage aléatoire
        srand((unsigned int)time(NULL));
        int tirage = rand() % 2; // 0 = monstre, 1 = loot

        if (tirage == 0) {
            printf("Attention ! Un monstre apparaît dans %s !\n", z->nom);
            int depth = z->profondeur;
            int nb;
            if (depth==50)
            {
                nb = (rand() % 2) + 1;
            }else if (depth==100)
            {
                nb = (rand() % 3) + 1;
            }else if (depth==150)
            {
                nb = (rand() % 4) + 1;
            }else{
                nb = (rand() % 5) + 1;
            }
            generateCreatureInTab(nb, depth);
            initFight(joueur, depth);
            cleanupAllCreatures();
            z->ennemis--;
        } else {
            printf("Vous trouvez un loot dans %s !\n", z->nom);
            // Objet loot = genererLootAleatoire();
            // inv_ajouter_objet(&joueur->inv, &loot);
            z->tresor--;
        }

    } else if (z->ennemis > 0) {
        // Seulement un monstre
        printf("Attention ! Un monstre apparaît dans %s !\n", z->nom);
        int depth = z->profondeur;
        int nb;
        if (depth==50)
        {
            nb = (rand() % 2) + 1;
        }else if (depth==100)
        {
            nb = (rand() % 3) + 1;
        }else if (depth==150)
        {
            nb = (rand() % 4) + 1;
        }else{
            nb = (rand() % 5) + 1;
        }
        generateCreatureInTab(nb, depth);
        initFight(joueur, depth);
        cleanupAllCreatures();
        z->ennemis--;

    } else if (z->tresor > 0) {
        // Seulement un loot
        printf("Vous trouvez un loot dans %s !\n", z->nom);
        // Objet loot = genererLootAleatoire();
        // inv_ajouter_objet(&joueur->inv, &loot);
        z->tresor = 0;
    }

}

int fin_jeu(Inventaire *inv){
    if (!inv) return 0;
    for (int i = 0; i < inv->nb_objets; i++) {
        Objet *o = &inv->slots[i];
        if (o->type == OBJ_CARTE && o->data.carte.niveau == 4 && o->quantite > 0) {
            return 1; // carte trouvée
        }
    }
    return 0;
}