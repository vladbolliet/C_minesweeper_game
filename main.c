#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "grille.h"
#include "pion.h"
#include "ui.h"
#include "histoire.h"

int main(){

    regles_du_jeu();

    int option; 

    // saisir option

    while(1){
        printf("Saisir 1 pour mode histoire, ou 2 pour mode libre\n");
        scanf("%d", &option);
        if(option==1 || option==2){break;}
        else{printf("Option invalide, reesayez\n");}
    }

    if(option == 1){ // mode histoire

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
            desallouer_pion(&p);

        }

        // histoire 2

        episode2();

    }

    else{

        Pion* p = allouer_pion();
        Grille* g = NULL;

        int l, c;

        // saisir dimensions
        
        while(1){
            printf("Saisir dimensions pour la grille (au moins 5x6 ou 6x5)\n");
            scanf("%d %d", &l, &c);
            g = creer_grille(l, c);
            if(g==NULL){printf("reesayez\n"); continue;}
            else{break;}
        }

        p->x = g->nb_lignes/2;
        p->y = g->nb_colonnes/2;

        jeu(g, p);

        desallouer_grille(&g);
        desallouer_pion(&p);

    }
    
    // fin du programme

    endwin();
    
    return 0;

}