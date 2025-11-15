#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "creature/creature.h"
#include "combat/combat.h"
#include "joueur/joueur.h"
#include "carte/carte.h"
#include "./inventaire/inventaire.h"

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
    srand(time(NULL));
    char *choix = malloc(sizeof(char)*100);
    int fin = 1;

    int depth = 49;

    int action = 0;
    afficher_intro();
    scanf("%s", choix);
    Zone *zone = addZone();//création des zone

    while (fin) {//boucle tant que le jeu n'ai pas fini
        if (strcmp(choix, "oui") == 0 || strcmp(choix, "OUI") == 0) {
            printf("\nParfait ! Préparez votre harpon... l’aventure commence !\n");
            Plongeur *joueur = initializePlongeur(zone[0]);// création du joueur
            inv_init(&joueur->inv);
            // printPlongeur(joueur);//affichage du joueur
            action=printZone(joueur);
            if (action==1) {
                perteO2(joueur);//enléve l'oxygéne ou la santer si il n'y a plus d'oxygéne
                printPlongeur(joueur);
            }else if (action==2) {

            }else if (action==3) {

            }else if (action==4) {

            }

            generateCreatureInTab(2, depth);

            // item test
            Objet trousse = objet_trousse("Trousse_Soin", 1, 20);
            Objet capsule = objet_capsule("Capsule_O2", 1, 20);

            if (inv_ajouter_objet(&joueur->inv, &trousse) == 0) {
                printf("Trousse de soin ajoutee a l'inventaire.\n");
            } else {
                printf("Echec ajout trousse.\n");
            }

            if (inv_ajouter_objet(&joueur->inv, &capsule) == 0) {
                printf("Capsule O2 ajoutee a l'inventaire.\n");
            } else {
                printf("Echec ajout capsule O2.\n");
            }

            initFight(joueur, depth);

            cleanupAllCreatures();
            free(choix);
            fin = 0;//fin du jeu
        } else if (strcmp(choix, "non") == 0 || strcmp(choix, "NON") == 0) {
            printf("\nVous avez choisi de rester en surface. À bientôt ! \n");
            fin = 0;//fin du jeu
        } else {//répéte la question
            printf("\nRéponse invalide. Veuillez taper 'oui' ou 'non'.\n");
        }
    }

    cleanupAllCreatures();
    free(choix); 

    return 0;
}