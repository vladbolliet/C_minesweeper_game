#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "ui.h"
#include "grille.h"

void init_ncurses(){
    initscr(); // initialiser ncurses
    raw(); // les entrees au clavier vont au programme ncurses charactere par charactere (sans attendre la touche 'entrer')
    keypad(stdscr, TRUE); // activer fleches
    noecho();
    halfdelay(1); // attendre 0.1s pour l'entrer. Si aucune touche dans les 0.1s, getch() retourne ERR
}

void jeu(Grille* g, Pion* p){

    init_ncurses();
    refresh(); // je sais pas exactement ce qui se passe deriere, mais ca marche avec et ca ne marche pas sans, donc...
    imprimer_grille(g); // pour voir la grille avant de commencer le jeu

    int ch = -1; // -1 pour rentrer dans la boucle pour la premiere fois

    while(1){
        ch = getch();
        switch(ch){
            case KEY_UP:
                deplacer_pion(g,p,HAUT);
                imprimer_grille(g);
                break;
            case KEY_DOWN: 
                deplacer_pion(g,p,BAS);
                imprimer_grille(g);
                break;
            case KEY_LEFT: 
                deplacer_pion(g,p,GAUCHE);
                imprimer_grille(g);
                break;
            case KEY_RIGHT:
                deplacer_pion(g,p,DROITE);
                imprimer_grille(g);
                break;
            case 'e':
                effacer_cellule(g, p);
                if(g->cases[p->x][p->y].est_marque) {marquer_cellule(g, p->x, p->y); } // quand on marque une cellule vide et on l'efface, SANS CETTE LIGNE la case va rester rouge (meme si on "efface" la case)
                imprimer_grille(g); // pour updater immediatement lorsque les cellules s'effacent recursivement (SANS CETTE LIGNE la grille va se mettre a jour apres un mouvement, et pas directement apres qu'on efface)
                break;
            case 'r':
                if(!g->cases[p->x][p->y].est_revelee) { marquer_cellule(g, p->x, p->y); }
                imprimer_grille(g); // juste pour mettre a jour le compteur lorsqu'on marque la cellule, pas quand on bouge
                break;
            case 27: // esc
                endwin();
                printf("jeu quitte\n");
                exit(0);
        }
        if(nb_cases_non_revelees(g) == g->nb_lignes * g->nb_colonnes / 5){
            int imprimer_juste_une_fois = 1; // variable utile a attendre que le joueur appuie sur KEY_RIGHT sans imprimer "Niveau reussi..." toutes les 0.1s
            while(getch() != KEY_RIGHT) {
                if(imprimer_juste_une_fois) {
                    printf("Niveau reussi, tu as revele toutes les cases sans bombes!\33[1E");
                    printf("Tapez sur la fleche de droite pour continuer...\n");
                    fflush(stdout);
                    imprimer_juste_une_fois = 0;
                }
            }
            break;
        }
    }

}

void regles_du_jeu(){

    // necessaire pour imprimer le message a l'ecran et aussi activer ncurses

    init_ncurses();
    refresh();

    // titre

    printf("\033[1E"
        "███╗   ███╗██╗███╗   ██╗███████╗███████╗██║  ██║ ██║ ███████╗███████╗██████╗ ███████╗██████╗ \033[1E"
        "████╗ ████║██║████╗  ██║██╔════╝██╔════╝██║  ██║ ██║ ██╔════╝██╔════╝██╔══██╗██╔════╝██╔══██╗\033[1E"
        "██╔████╔██║██║██╔██╗ ██║█████╗  ███████╗██║  ██║ ██║ █████╗  █████╗  ██████╔╝█████╗  ██████╔╝\033[1E"
        "██║╚██╔╝██║██║██║╚██╗██║██╔══╝  ╚════██ ██╝  ██╝ ██║ ██╔══╝  ██╔══╝  ██╔═══╝ ██╔══╝  ██╔══██╗\033[1E"
        "██║ ╚═╝ ██║██║██║ ╚████║███████╗███████╗███████████║ ███████╗███████╗██║     ███████╗██║  ██║\033[1E"
        "╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝╚══════════╝ ╚══════╝╚══════╝╚═╝     ╚══════╝╚═╝  ╚═╝\033[1E"
        "\033[1E"
        "    ---------------------         L'histoire perdue     --------------------\033[1E\033[1E");        

    // regles

    printf("Il y aura une petite histoire\033[1E\033[1E");

    printf("Les instructions :\033[1E\033[1E");
    printf("Pour l'histoire, vous n'avez que a bouger avec les fleches du clavier\033[1E\033[1E");
    printf("Pour MINESWEEPER, apart bouger avec les fleches, il y a: \033[1E\033[1E");
    printf("- 'e' pour decouvrir le bloc courrant\033[1E");
    printf("- 'r' pour marquer une cellule comme ayant une bombe (n'affecte pas le jeu, est juste pour aider le joueur)\033[1E\033[1E");
    printf("Regles rapides de MINESWEEPER:\033[1E\033[1E");
    printf("- Le but est de decouvrir toutes les cellules ne contenant PAS une bombe (vous disposez d'un compteur pour voir combien de bombes il reste)\033[1E");
    printf("- Si vous decouvrez une cellule contenant une bombe, vous perdez\033[1E");
    printf("- Les cellules sont au debut du jeu couvertes et vous devez les decouvrir (sauf l'espace 3x3 deja decouvert sur lequel vous etes initialises)\033[1E");
    printf("- Un nombre sur une cellule represente le nombre EXACT de bombes autour de cette cellule\033[1E");
    printf("- Attention, il est possible que dans des cas particuliers vous ne pouvez pas deduire quelles cellules ont des bombes, vous devez deviner (comme dans MINSWEEPER original)\033[1E\033[1E");
    printf("Tapez la fleche de droite pour continuer!\033[1E");

    fflush(stdout);

    // attendre la saisie du joueur

    int ch;
    while(1){
        ch = getch();
        if(ch == KEY_RIGHT){ break; }
    }
    endwin();
    
}