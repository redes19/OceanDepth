#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "joueur/joueur.h"
#include "joueur/joueur.c"

void afficher_intro() {
    printf("=====================================\n");
    printf("        Bienvenue dans OceanDepth \n");
    printf("=====================================\n");
    printf("\n");
    printf("Souhaitez-vous commencer une nouvelle partie ? (oui/non)\n");
    printf("> ");
}
int main()
{
    char *choix = malloc(sizeof(char)*100);
    int fin = 1;
    afficher_intro();
    scanf("%s", choix);

    while (fin) {//boucle tant que le jeu n'ai pas fini
        if (strcmp(choix, "oui") == 0 || strcmp(choix, "OUI") == 0) {
            Plongeur *joueur = initializePlongeur();// création du joueur
            printPlongeur(joueur,50);//affichaage du joueur
            fin = 0;//fin du jeu
        } else if (strcmp(choix, "non") == 0 || strcmp(choix, "NON") == 0) {
            printf("\nVous avez choisi de rester en surface. À bientôt ! \n");
            fin = 0;//fin du jeu
        } else {//répéte la question
            printf("\nRéponse invalide. Veuillez taper 'oui' ou 'non'.\n");
        }
    }


    return 0;
}