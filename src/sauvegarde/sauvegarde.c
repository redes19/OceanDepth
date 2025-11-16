#include <stdio.h>
#include <string.h>
#include "sauvegarde.h"

 #include "../creature/creature.h"


// ------------------------------
// Implémentation portable strtok_r pour MinGW / Windows
// ------------------------------
#if defined(_WIN32) && !defined(strtok_r)

char *strtok_r(char *str, const char *delim, char **saveptr)
{
    char *start;
    char *end;

    if (str != NULL) {
        *saveptr = str;
    }

    start = *saveptr;
    if (start == NULL)
        return NULL;

    // Passer les délimiteurs initiaux
    start += strspn(start, delim);
    if (*start == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    // Trouver la fin du token
    end = start + strcspn(start, delim);
    if (*end == '\0') {
        *saveptr = NULL;
    } else {
        *end = '\0';
        *saveptr = end + 1;
    }

    return start;
}
#endif


// =============================================================
// SAUVEGARDE – IMPLÉMENTATION
// Choix pédagogique :
// - On écrit un format très lisible (balises en clair)
// - L'inventaire est externalisé dans save.inv (réutilise les fonctions existantes)
// =============================================================

int save_game(const char *path, const Plongeur *p, const Position *pos,
const CreatureMarine *creatures, int nb)
{
    if (!path || !p || !pos) return -1;
    FILE *f = fopen(path, "w");
    if (!f) return -1;


    fprintf(f, "OCEANDEPTHS_SAVE_V1\n");


    // PLONGEUR:PV:PV_MAX:O2:O2_MAX:FATIGUE:PERLES
    fprintf(f, "PLONGEUR:%d:%d:%d:%d:%d:%d\n",
    p->points_de_vie, p->points_de_vie_max,
    p->niveau_oxygene, p->niveau_oxygene_max,
    p->niveau_fatigue, p->inv.perles);


    // POSITION actuelle
    fprintf(f, "POSITION:%d:%s\n", pos->profondeur, pos->zone);


    // Liste des créatures (utile si on sauvegarde en plein combat / grotte)
    fprintf(f, "CREATURES:%d\n", nb);
    for (int i = 0; i < nb; ++i) {
        const CreatureMarine *c = &creatures[i];
        // Adapter aux champs réels de votre struct CreatureMarine
        fprintf(f, "C:%d:%s:%d:%d:%d:%d:%d\n", c->id, c->name,
        c->life, c->max_life, c->vitesse, c->min_attack, c->max_attack);
    }


    // Inventaire : on inclut juste le nom du fichier compagnon
    fprintf(f, "INVENTAIRE:INCLUDE:save.inv\n");


    fclose(f);


    // On sauvegarde l'inventaire dans le fichier compagnon
    return inv_sauvegarder_fichier(&p->inv, "save.inv");
}

int load_game(const char *path, Plongeur *p, Position *pos,
CreatureMarine *creatures, int *nb, int capacity)
{
    if (!path || !p || !pos || !creatures || !nb) return -1;
    FILE *f = fopen(path, "r");
    if (!f) return -1;


    char line[512];
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }
    if (strncmp(line, "OCEANDEPTHS_SAVE_V1", 19) != 0) { fclose(f); return -1; }


    // PLONGEUR
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }
    sscanf(line, "PLONGEUR:%d:%d:%d:%d:%d:%d",
    &p->points_de_vie, &p->points_de_vie_max,
    &p->niveau_oxygene, &p->niveau_oxygene_max,
    &p->niveau_fatigue, &p->inv.perles);

    // POSITION
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }
    {
        int prof = 0; char zone[64]="";
        // format : POSITION:profondeur:zone (zone = fin de ligne)
        sscanf(line, "POSITION:%d:63[^\n]", &prof, zone);
        pos->profondeur = prof;
        strncpy(pos->zone, zone, sizeof(pos->zone)-1); pos->zone[sizeof(pos->zone)-1]=0;
    }

    // CREATURES
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }
    int n = 0;
    sscanf(line, "CREATURES:%d", &n);
    if (n > capacity) n = capacity; // sécurité si tableau trop petit


    for (int i = 0; i < n; ++i) {
        if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }
        // C:id:name:life:max_life:vitesse:atk_min:atk_max
        CreatureMarine *c = &creatures[i];
        int id, life, max_life, vit, atk_min, atk_max;
        char name[64];
        if (sscanf(line, "C:%d:63[^:]:%d:%d:%d:%d:%d",
        &id, name, &life, &max_life, &vit, &atk_min, &atk_max) == 7) {
            c->id = id; strncpy(c->name, name, sizeof(c->name)-1); c->name[sizeof(c->name)-1]=0;
            c->life = life; c->max_life = max_life; c->vitesse = vit;
            c->min_attack = atk_min; c->max_attack = atk_max;
            c->life = (life > 0);
        }
    }
    *nb = n;

    // INVENTAIRE compagnon
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }
    // format : INVENTAIRE:INCLUDE:save.inv
    char include[64]="";
    if (sscanf(line, "INVENTAIRE:INCLUDE:%63s", include) == 1) {
        if (inv_charger_fichier(&p->inv, include) != 0) {
            fclose(f); return -1;
        }
    }


    fclose(f);
    return 0;
}