#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "creature/creature.h"

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

    int depth = 49;

    afficher_intro();
    scanf("%s", choix);
    while (fin) {
        if (strcmp(choix, "oui") == 0 || strcmp(choix, "OUI") == 0) {
            printf("\nParfait ! Préparez votre harpon... l’aventure commence !\n");
            printf("Name of Monster : \n");
            createCreature(depth);
            fin = 0;
        } else if (strcmp(choix, "non") == 0 || strcmp(choix, "NON") == 0) {
            printf("\nVous avez choisi de rester en surface. À bientôt ! \n");
            fin = 0;
        } else {
            printf("\nRéponse invalide. Veuillez taper 'oui' ou 'non'.\n");
        }
    }


    return 0;
}