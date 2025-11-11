@echo off
echo Compilation du projet OceanDepth...
gcc -Wall -Wextra -std=c99 -O2 -o oceandepth.exe src/main.c src/joueur/joueur.c src/creature/creature.c src/combat/combat.c src/carte/carte.c src/inventaire/inventaire.c -lm

if %errorlevel% equ 0 (
    echo Compilation reussie! L'executable oceandepth.exe a ete cree.
    echo.
    echo Pour lancer le jeu, tapez: oceandepth.exe
) else (
    echo Erreur lors de la compilation.
)
pause