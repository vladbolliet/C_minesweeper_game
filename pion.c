#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>  // pour 'usleep'
#include "pion.h"
#include "grille.h"
#include "ui.h"

Pion* allouer_pion(){
    Pion* p = (Pion*) malloc(sizeof(Pion));
    if(!p) {printf("allocation echouee\n"); return NULL;}
    return p;
}

void desallouer_pion(Pion** p){
    free(*p);
    *p = NULL;
}

void bouger_pion_logic(Grille* g, Pion* p, int a, int b){

    // si les coordonnees sont en dehors de la grille

    if(a > g->nb_lignes - 1 || a < 0 || b > g->nb_colonnes - 1 || b < 0){return;}

    // on verifie la nature de la destination 

    type_cellule dest = g->cases[a][b].type; // quel est le bloc destination?

    switch(dest){
        case RIEN: // les 3 cases sont comme un 'case RIEN || BOMBE'
        case BOMBE: // faut pas oublier que en dessous, la case peut etre de type BOMBE... meme si la cellule n'est pas relevee
        case NOMBRE:
            p->x_precedent = p->x;
            p->y_precedent = p->y;
            p->x = a;
            p->y = b;
            g->cases[a][b].pion_est_dessus = TRUE;
            g->cases[p->x_precedent][p->y_precedent].pion_est_dessus = FALSE;
            break;
    }
}

void deplacer_pion(Grille* g, Pion* p, evenement_pion even){
    switch(even){
        case HAUT:
            bouger_pion_logic(g,p,p->x-1, p->y);
            break;
        case BAS:
            bouger_pion_logic(g,p,p->x+1, p->y);
            break;
        case GAUCHE:
            bouger_pion_logic(g,p,p->x, p->y-1);
            break;
        case DROITE:
            bouger_pion_logic(g,p,p->x, p->y+1);
            break;
    }
}

void effacer_cellule(Grille* g, Pion* p){

    // si joueur efface un bloc vide

    if(g->cases[p->x][p->y].type == RIEN){
        g->cases[p->x][p->y].est_revelee = TRUE;
        g->cases[p->x][p->y].nb_bombes_voisignage = calculer_nombre_cellule(g, p->x, p->y);
        if(g->cases[p->x][p->y].type == RIEN) { // si quand on efface la cellule cela ne contient pas un nombre, on efface recursivement toutes les cases vides adjacentes
            effacer_cellules_adjacentes(g, p->x, p->y);
        }
    }

    // si joueur efface une bombe

    else if (g->cases[p->x][p->y].type == BOMBE){

        // animation si tu as perdu

        g->cases[p->x][p->y].pion_est_dessus = FALSE; // colorer bloc bleu en rouge (juste pour l'estetique)

        for(int i=0; i<g->nb_lignes; i++){
            for(int j=0; j<g->nb_colonnes; j++){
                g->cases[i][j].est_revelee = TRUE;
                imprimer_grille(g);
                usleep(25000); // attendre 0.025s
            }
        }

        printf("\nperdu, tu as touche une bombe!\033[E1");
        
        //refresh();

        // finir le jeu
        
        init_ncurses();
        printf("appuyez 'esc' pour quitter\n");
        fflush(stdout);
        while(getch() != 27){}
        endwin();
        exit(0);
    }
}

void effacer_cellules_adjacentes(Grille* g, int a, int b){

    // quand on rentre dans la fonction, on sait deja que g[a][b].type == RIEN

    int i, j;

    for (i = a - 1; i <= a + 1; i++){
        for (j = b - 1; j <= b + 1; j++){
            if (i < 0 || i >= g->nb_lignes || j < 0 || j >= g->nb_colonnes) { continue; } // si la case est en dehors de la grille, on continue
            if(g->cases[i][j].type == RIEN && !g->cases[i][j].est_revelee){
                g->cases[i][j].est_revelee = TRUE;
                g->cases[i][j].nb_bombes_voisignage = calculer_nombre_cellule(g, i, j);
                if(g->cases[i][j].type == RIEN) { effacer_cellules_adjacentes(g, i, j); }
            }
        }
    }
}

void marquer_cellule(Grille* g, int a, int b){
    if(g->cases[a][b].est_marque){
        g->cases[a][b].est_marque = FALSE;
    }
    else{
        g->cases[a][b].est_marque = TRUE;
    }
}