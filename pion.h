#ifndef PION_H
#define PION_H

#include "grille.h"

typedef enum {HAUT, BAS, DROITE, GAUCHE} evenement_pion;

typedef struct{
    int x, y; // coordonnees courantes
    int x_precedent, y_precedent; // coordonnees precedentes
} Pion;

Pion* allouer_pion();
void desallouer_pion(Pion** p);
void bouger_pion_logic(Grille* g, Pion* p, int a, int b); // fonction auxiliare de deplacer_pion
void deplacer_pion(Grille* g, Pion* p, evenement_pion even); // l'interface agreable pour celui qui programme, en utilisant evenement_pion
void effacer_cellule(Grille* g, Pion* p); // joueur appuit 'e' pour effacer le cellule sur laquelle il est
void effacer_cellules_adjacentes(Grille* g, int a, int b); // si quand on efface une cellule cela ne contient pas un nombre, on efface recursivement toutes les cases vides adjacentes
void marquer_cellule(Grille* g, int a, int b); // marquer une cellule comme ayant une bombe 

#endif