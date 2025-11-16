#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "creature/creature.h"
#include "combat/combat.h"
#include "joueur/joueur.h"
#include "carte/carte.h"
#include "sauvegarde/sauvegarde.h"
#include "inventaire/inventaire.h"

void afficher_intro() {
    printf("=====================================\n");
    printf("        Bienvenue dans OceanDepth \n");
    printf("=====================================\n");
    printf("\n");
    printf("Souhaitez-vous commencer une nouvelle partie ? (oui/non)\n");
    printf("> ");
}

int main() {
    srand(time(NULL));
    char choix[100];
    int fin = 1;

    int depth = 49;

    int action = 0;

    // Création de la carte et du joueur
    Zone **carte = newCarte();
    Plongeur *joueur = initializePlongeur(&carte[0][0]);
    afficher_intro();
    scanf("%s", choix);
    if (strcmp(choix, "oui") == 0) {
        printPlongeur(joueur);
        while (fin) {
            action = printZone(joueur, carte);
            if (action == 1) {               
                deplacement(joueur,carte);
                perteO2(joueur);
            }
            else if (action == 2) {
                explorerZone(joueur);
            }
            else if (action == 3) {
                
                fin = 0;
            }
            else if (action == 4) {
                printCarte(joueur, carte);
            }
            else {
                printf("Action invalide.\n");
            }
            if(fin_jeu(&joueur->inv)){
                fin=0;
            }
        }
    }else if (strcmp(choix, "non") == 0) {
        printf("\nVous avez choisi de rester en surface. À bientot ! \n");
        
    }

    

    // Libération de la mémoire de la carte
    for (int i = 0; i < 4; i++) {
        free(carte[i]);
    }
    free(carte);

    // Libération du joueur
    free(joueur);

    return 0;
}