#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "grille.h"
#include "pion.h"
#include "ui.h"
#include "histoire.h"

int main(){

    regles_du_jeu();

    // histoire 1
    
    episode1();

    // niveaux MINESWEEPER

    Pion* p = allouer_pion(); 

    for(int i=6; i<=12; i=i+3){

        Grille* g = creer_grille(i, i);
        p->x = g->nb_lignes/2;
        p->y = g->nb_colonnes/2;

        jeu(g, p);

        desallouer_grille(&g);

    }

    // histoire 2

    episode2();
    
    // fin du programme

    desallouer_pion(&p);

    endwin();
    
    return 0;

}