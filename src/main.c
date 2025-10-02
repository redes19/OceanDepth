#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "joueur/joueur.h"
#include "joueur/joueur.c"
#include "carte/carte.h"
#include "carte/carte.c"

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
    int action = 0;
    afficher_intro();
    scanf("%s", choix);
    Zone *zone = addZone();//création des zone

    while (fin) {//boucle tant que le jeu n'ai pas fini
        if (strcmp(choix, "oui") == 0 || strcmp(choix, "OUI") == 0) {
            Plongeur *joueur = initializePlongeur(zone[0]);// création du joueur
            printPlongeur(joueur);//affichage du joueur
            action=printZone(joueur);
            if (action==1) {
                perteO2(joueur);//enléve l'oxygéne ou la santer si il n'y a plus d'oxygéne
                printPlongeur(joueur);
            }else if (action==2) {

            }else if (action==3) {

            }else if (action==4) {

            }
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