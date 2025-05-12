#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "grille.h"
#include "pion.h" // pour effacer_cellules_adjacentes

Grille* creer_grille(int nb_lignes, int nb_colonnes){

    if((nb_lignes < 5 || nb_colonnes <  6) && (nb_colonnes < 5 || nb_lignes < 6 ) ) {printf("grille trop petite\n"); exit(0); return NULL;} // j'ai teste, et la plus petite grille possible est de taille 6x5 (ou 5x6)

    int i, j; // variables de boucle

    // allouer la grille

    Grille* g = (Grille*) malloc(sizeof(Grille));
    if( !g ) { printf("allocation grille echouee\n"); return NULL; }
    g->cases = (Cellule**) malloc(nb_lignes * sizeof(Cellule*));
    if (!g->cases) {printf("allocation grille echouee\n"); free(g); return NULL;}
    for (i = 0; i < nb_lignes; i++){
        g->cases[i] = (Cellule*) malloc ( nb_colonnes * sizeof(Cellule));
        if( !g->cases[i] ) {
            for (j = 0; j < i; j++){ free(g->cases[j]); }
            free(g->cases);
            free(g);
            return NULL;
        }
    }
    g->nb_lignes = nb_lignes;
    g->nb_colonnes = nb_colonnes;

    // la remplir avec des cellules vides ET mettre le statut de toutes les cellules a non relevees

    for(i = 0; i < nb_lignes; i++){
        for(j = 0; j < nb_colonnes; j++){
            g->cases[i][j].type = RIEN;
            g->cases[i][j].est_revelee = FALSE;
            g->cases[i][j].pion_est_dessus = FALSE;
            g->cases[i][j].nb_bombes_voisignage = 0;
            g->cases[i][j].est_marque = FALSE;
        }
    }

    // aleatoirement mettre bombes sur la grille

    srand(time(NULL));

    int nb_bombes = nb_lignes*nb_colonnes/5, ri, rj; // y a des bombes sur 20% de la grille

    while (nb_bombes){
        ri = rand()%nb_lignes;
        rj = rand()%nb_colonnes;
        if (g->cases[ri][rj].type != BOMBE && cellule_pas_entournee_par_bombes(g, ri, rj) && pas_sur_case_de_depart(g, ri, rj)){
            g->cases[ri][rj].type = BOMBE;
            nb_bombes--;
        }
    }

    // initialiser le bloc de 3x3 autour du joueur et mettre nombres sur la grille de depart (nomalement on le fait que quand le joueur efface un bloc, mais il faut initialiser le bloc de depart aussi)

    int coordonnees_initialises_pion_x = nb_lignes/2, coordonnees_initialises_pion_y = nb_colonnes/2;

    g->cases[coordonnees_initialises_pion_x][coordonnees_initialises_pion_y].pion_est_dessus = TRUE; // initialiser le pion au millieu de la grille

    for(i = coordonnees_initialises_pion_x - 1 ; i<= coordonnees_initialises_pion_x + 1; i++){
        for (j = coordonnees_initialises_pion_y - 1; j <= coordonnees_initialises_pion_y + 1; j++){
            g->cases[i][j].est_revelee = TRUE;
            g->cases[i][j].nb_bombes_voisignage = calculer_nombre_cellule(g, i, j);
            effacer_cellules_adjacentes(g, i, j);
        }
    }

    return g;
}

int pas_sur_case_de_depart(Grille* g, int ri, int rj){ // pas mettre bombes sur les cases de depart

    int i, j;
    
    for(i = g->nb_lignes/2 - 1 ; i<= g->nb_lignes/2 + 1; i++){
        for (j = g->nb_colonnes/2 - 1; j <= g->nb_colonnes/2 + 1; j++){
            if(ri == i && rj == j){
                return 0; // faux
            }
        }
    }
    
    return 1; // true
}

int cellule_pas_entournee_par_bombes(Grille* g, int ri, int rj){ // verifier si autour de ce bloc il y a que des bombes (meme a la periferie)

    int i, j;
    
    for (i = ri - 1; i <= ri + 1; i++){
        for (j = rj - 1; j <= rj + 1; j++){
            if (i < 0 || i >= g->nb_lignes || j < 0 || j >= g->nb_colonnes) { continue; } // si la case est en dehors de la grille, on continue
            if(g->cases[i][j].type != BOMBE){
                return 1; // vrai
            }
        }
    }

    return 0; // faux

}

void imprimer_grille(Grille* g){

    printf("\33[2J"); // effacer ecran
    printf("\33[H"); // ecrire message en haut a gauche

    // imprimer le compteur de bombes

    int nb_marquees = nb_bombes_marquees(g);
    printf("Bombes marquees: %d\33[1EBombes restantes: %d\33[1E\33[1E", nb_marquees, (g->nb_lignes * g->nb_colonnes/5)-nb_marquees);

    // imprimer la grille

    for (int k = 0; k < g->nb_colonnes; k++) { printf("----"); }
    printf("-");

    for (int i = 0; i < g->nb_lignes; i++) {
        printf("\33[1E"); // retour a la ligne
        for (int j = 0; j < g->nb_colonnes; j++) {
            printf("|");
            if(g->cases[i][j].pion_est_dessus){ // si pion est sur case
                printf("\33[44m   \x1b[0m"); // bleu
            }
            else { // si pion n'est pas sur case
                if (g->cases[i][j].est_marque) {
                    printf("\x1b[41m   \x1b[0m"); // rouge 
                }
                else if (g->cases[i][j].est_revelee == FALSE) {
                    printf("\x1b[100m   \x1b[0m"); // gri
                }
                else{
                    switch(g->cases[i][j].type){
                        case RIEN:
                            printf("   "); // noir
                            break;
                        case BOMBE: // TODO: cazul asta e inutil...
                            printf("\x1b[41m   \x1b[0m"); // rouge
                            break;
                        case NOMBRE:
                            switch(g->cases[i][j].nb_bombes_voisignage){
                                case 0:
                                    printf("   "); // noir
                                    break;
                                case 1: 
                                    printf("\x1b[34m 1 \x1b[0m"); // bleu 1
                                    break;
                                case 2:
                                    printf("\x1b[32m 2 \x1b[0m"); // vert 2
                                    break;
                                case 3:
                                    printf("\x1b[35m 3 \x1b[0m"); // violet 3
                                    break;
                                case 4:
                                    printf("\x1b[36m 4 \x1b[0m"); // cyan 4
                                    break;
                            }
                            break;
                    }
                }
            }
            
        }
        printf("|\33[1E");
    }


    for (int j = 0; j < g->nb_colonnes; j++) printf("----");
    printf("-\33[1E");
    fflush(stdout); // forcement imprimer ce qu'il y a dans le buffer de stdout apres chaque mouvement

}

int calculer_nombre_cellule(Grille* g, int a, int b){

    int i, j, compteur = 0;

    for (i = a - 1; i <= a + 1; i++){
        for (j = b - 1; j <= b + 1; j++){
            if (i < 0 || i >= g->nb_lignes || j < 0 || j >= g->nb_colonnes) continue; // si la case est en dehors de la grille, on continue
            if (g->cases[i][j].type == BOMBE) compteur++;
        }
    }

    g->cases[a][b].nb_bombes_voisignage = compteur;
    if (compteur > 0) g->cases[a][b].type = NOMBRE; // le bloc de type 'rien' qui vient d'etre efface devient une cellule de type nombre

    return compteur;
}


int nb_bombes_marquees(Grille* g){
    int i, j, compteur = 0;
    for(i = 0; i < g->nb_lignes; i++){
        for(j = 0; j < g->nb_colonnes; j++){
            if(g->cases[i][j].est_marque){
                compteur++;
            }
        }
    }
    return compteur;
}

int nb_cases_non_revelees(Grille* g){
    int compteur = 0, i, j;
    for(i = 0; i < g->nb_lignes; i++){
        for(j = 0; j < g->nb_colonnes; j++){
            if(!g->cases[i][j].est_revelee){
                compteur++;
            }
        }
    }
    return compteur;
}

void desallouer_grille(Grille** g){
    for(int i = 0; i < (*g)->nb_lignes ; i++){
        free((*g)->cases[i]);
    }
    free((*g)->cases);
    free(*g);
    *g = NULL;
}