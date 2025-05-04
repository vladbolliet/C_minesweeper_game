#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>  // pour 'usleep'
#include "histoire.h"
#include "ui.h" // pour init_ncurses();

// grille

Grille_histoire* Grille_initialiser_H(int n, int m){
    Grille_histoire* g = (Grille_histoire*) malloc(sizeof(Grille_histoire)); // allouer structure principale
    if (!g) { printf("erreur d'allocation\n"); return NULL; }
    g->n = n;
    g->m = m;
    g->grille = (element_grille**) malloc(sizeof(element_grille*) * n); // allouer tableau de pointeurs sur element_grille 
    if (!g->grille) {
        printf("erreur d'allocation\n");
        free(g);
        return NULL;
    }

    // allouer chaque pointeur sur element_grille
    for (int i = 0; i < n; i++) {
        g->grille[i] = (element_grille*) malloc(sizeof(element_grille) * m);
        if (!g->grille[i]) {
            printf("erreur d'allocation\n");
            for (int j = 0; j < i; j++) free(g->grille[j]); // desalouer les rends alloues avant
            free(g->grille);
            free(g);
            return NULL;
        }
    }

    // pas besoin de initialser les cellules a RIEN_H, on le fera quand on lit des fichiers

    return g;
}



void Grille_desallouer_H(Grille_histoire** g){
    int i;
    for(i=0; i<(*g)->n; i++){
        free((*g)->grille[i]);
        (*g)->grille[i] = NULL;
    }
    free((*g)->grille);
    (*g)->grille = NULL;
    free(*g);
    *g = NULL;
}

void Grille_redessiner_H(Grille_histoire* g){
    printf("\33[2J"); // effacer ecran
    printf("\33[H"); // ecrire message en haut a gauche
    for(int i=0; i<g->n; i++){
        for(int j=0; j<g->m; j++){
            switch(g->grille[i][j]){
                case RIEN_H:
                    printf("%s", "\33[40m  ");  /* Affiche un carré noir */
                    break;
                case MUR_H:
                    printf("%s", "\33[47m  ");  /* Affiche un carré blanc */
                    break;
                case PION_BLEU_H:
                    printf("%s", "\33[44m  ");  /* Affiche un carré bleu */
                    break;
                case PION_ROSE_H:
                    printf("%s", "\33[45m  "); /* Affiche un carré magenta */
                    break;
                case PION_MECHANT_H:
                    printf("%s", "\33[48;5;52m  "); /* Affiche un carré rouge fonce */
                    break;
                default:
                    printf("%s", "\33[40m  "); /* Default to black if unknown value */
                    break;
            }
        }
        printf("\33[1E"); // retour a la ligne
    }
    printf("\33[40m"); // reinitialiser texte au noir
    fflush(stdout);
}

Grille_histoire* Grille_charger_fichier_H(char* nom_fic){

    // ouvrir fichier

    FILE* f = fopen(nom_fic, "r");
    if(!f){ printf("fichier n'existe pas\n"); return NULL; }

    // lire dimensions

    int n, m;
    fscanf(f, "%d %d\n", &n , &m);
    Grille_histoire* g = Grille_initialiser_H(n, m);

    // lire contenu

    char c;
    int i = 0, j = 0;

    while(1){

        if (i > g->n + 1 || j > g->m + 1) {
            printf("trop de données\n");
            break;
        }

        c = fgetc(f); // lire charactere 1 par 1
        if (c == EOF) {
            break;
        }

        if (c == 'M'){ // mur
            g->grille[i][j] = MUR_H; j++;
        }
        else if (c == 'B'){
            g->grille[i][j] = PION_BLEU_H; j++;
        }
        else if (c == 'R'){
            g->grille[i][j] = PION_ROSE_H; j++;
        }
        else if (c == 'P'){
            g->grille[i][j] = PION_MECHANT_H; j++;
        }
        else if (c == ' '){
            g->grille[i][j] = RIEN_H; j++;
        }
        else if (c == '\n'){
            i++;
            j = 0;
        }
    }

    fclose(f);
    return g;

}

// pion

Pion_histoire* allouer_pion_H(){
    Pion_histoire* p = (Pion_histoire*) malloc(sizeof(Pion_histoire));
    if(!p) {printf("allocation echouee\n"); return NULL;}
    return p;
}

void desallouer_pion_H(Pion_histoire** p){
    free(*p);
    *p = NULL;
}

void bouger_pion_logic_H(Grille_histoire* g, Pion_histoire* p, int a, int b){

    // si les coordonnees sont en dehors de la grille

    if(a > g->n - 1 || a < 0 || b > g->m - 1 || b < 0){return;}

    // on verifie la nature de la destination 

    element_grille dest = g->grille[a][b]; // quel est le bloc destination?

    switch(dest){
        case RIEN_H:
            p->x_precedent = p->x;
            p->y_precedent = p->y;
            p->x = a;
            p->y = b;
            g->grille[p->x_precedent][p->y_precedent] = RIEN_H;
            g->grille[a][b] = PION_BLEU_H;
            break;
        case MUR_H:
            p->x = p->x_precedent;
            p->y = p->y_precedent;
            return;
        case PION_ROSE_H:
            //TODO: gagner et animation coeur (le joueur va toucher le pion rose a la fin du jeu...)
    }
}

void deplacer_pion_H(Grille_histoire* g, Pion_histoire* p, evenement_pion_H even){
    p->x_precedent = p->x;
    p->y_precedent = p->y;
    switch(even){
        case HAUT_H:
            bouger_pion_logic_H(g,p,p->x-1, p->y);
            break;
        case BAS_H:
            bouger_pion_logic_H(g,p,p->x+1, p->y);
            break;
        case GAUCHE_H:
            bouger_pion_logic_H(g,p,p->x, p->y-1);
            break;
        case DROITE_H:
            bouger_pion_logic_H(g,p,p->x, p->y+1);
            break;
    }
}

void imprimer_texte_avec_animation(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);      // imprimer 1 charactere
        fflush(stdout);       
        usleep(50000);        // attendre 0.05s par charactere affiche
    }
    putchar('\n');  
}

void episode1(){

    // frame 1

    Grille_histoire* gh = Grille_charger_fichier_H("./episodes/episode1/frame1.txt");

    Grille_redessiner_H(gh);

    printf("\033[1E");

    imprimer_texte_avec_animation("Il était une fois un bloc bleu et un bloc rose, heureux ensemble...");
    
    usleep(2000000); // attendre 2s
    
    printf("\033[1E");

    imprimer_texte_avec_animation("Un jour, un bloc maléfique arriva, vola le bloc rose et l'emmena.");

    usleep(2000000); // attendre 2s

    Grille_desallouer_H(&gh);

    // frame 2 - 20

    for(int i = 2; i<=20; i++){
        char nomfichier[100];
        sprintf(nomfichier, "./episodes/episode1/frame%d.txt", i); // sprintf pour formatter du texte dans un string
        gh = Grille_charger_fichier_H(nomfichier);

        usleep(200000); // 0.2s

        Grille_redessiner_H(gh);

        Grille_desallouer_H(&gh);
    }


    
    gh = Grille_charger_fichier_H("./episodes/episode1/frame20.txt"); // on reprend le dernier frame pour suivre les autres blocs

    usleep(1500000); // attendre 1.5s

    imprimer_texte_avec_animation("\033[1ELe bloc mechant mit des bombes sur la route. Le bloc bleu dois attentivement traverser 3 teritoires dangereux et sauver le bloc rose !\033[1E");

    usleep(2000000); // attendre 4s

    imprimer_texte_avec_animation("A toi de les suivre !\033[1E");

    usleep(2000000); // attendre 2s

    Pion_histoire* ph = allouer_pion_H(); 

    // trouver coordonnees du pion dans la grille pour savoir ou il commence dans le jeu

    for(int i = 0; i < gh->n; i++){
        for(int j=0; j < gh->m; j++){
            if(gh->grille[i][j] == PION_BLEU_H){
                ph->x = i;
                ph->y = j;
            }
        }
    }

    fflush(stdout);

    init_ncurses();
    refresh(); // je sais pas exactement ce qui se passe deriere, mais ca marche avec et ca ne marche pas sans, donc...
    Grille_redessiner_H(gh);

    int ch;

    while(1){
        ch = getch();
        switch(ch){
            case KEY_UP:
                deplacer_pion_H(gh,ph,HAUT);
                Grille_redessiner_H(gh);
                break;
            case KEY_DOWN: 
                deplacer_pion_H(gh,ph,BAS);
                Grille_redessiner_H(gh);
                break;
            case KEY_LEFT: 
                deplacer_pion_H(gh,ph,GAUCHE);
                Grille_redessiner_H(gh);
                break;
            case KEY_RIGHT:
                deplacer_pion_H(gh,ph,DROITE);
                Grille_redessiner_H(gh);
                break;
            case 27: // esc
                endwin();
                printf("Jeu quitte\n");
                exit(0);
        }
        if(ph->x == 0 && ph->y == 12){
            //refresh();
            printf("Tapez la fleche de droite pour continuer...");
            fflush(stdout);
            while(1){
                ch = getch();
                if(ch == KEY_RIGHT){break;}
            }
            break;
        }
    }
    
    endwin();
    fflush(stdout);
}

void episode2(){

    for(int i=1; i<=4; i++){

        char nomfichier[100];
        sprintf(nomfichier, "./episodes/episode2/frame%d.txt", i); // sprintf pour formatter du texte dans un string
        Grille_histoire* gh = Grille_charger_fichier_H(nomfichier);

        usleep(1000000); // 1s

        Grille_redessiner_H(gh);

        Grille_desallouer_H(&gh);
    }

    usleep(1000000); // 1s

    printf("\033[1E");

    imprimer_texte_avec_animation("Après un long voyage, le bloc bleu arriva enfin dans la cachette du bloc maléfique.\033[1E");

    usleep(1500000); // 1.5s

    imprimer_texte_avec_animation("Un dernier combat l'attendait.\033[1E");

    for(int i=5; i<=7; i++){
        char nomfichier[100];
        sprintf(nomfichier, "./episodes/episode2/frame%d.txt", i); // sprintf pour formatter du texte dans un string
        Grille_histoire* gh = Grille_charger_fichier_H(nomfichier);

        usleep(1000000); // 1s

        Grille_redessiner_H(gh);

        Grille_desallouer_H(&gh);
    }

    usleep(1000000); // 1s

    imprimer_texte_avec_animation("\033[1E...\033[1E");

    usleep(2000000); // 2s

    for(int i=8; i<=19; i++){
        char nomfichier[100];
        sprintf(nomfichier, "./episodes/episode2/frame%d.txt", i); // sprintf pour formatter du texte dans un string
        Grille_histoire* gh = Grille_charger_fichier_H(nomfichier);

        usleep(300000); // 0.3s

        Grille_redessiner_H(gh);

        Grille_desallouer_H(&gh);
    }

    usleep(1000000); // 1s

    printf("\033[1E");

    imprimer_texte_avec_animation("Après un combat épuisant, le bloc bleu réussit à porter le coup final.\033[1E");

    usleep(500000); // 0.5s

    for(int i=20; i<=21; i++){
        char nomfichier[100];
        sprintf(nomfichier, "./episodes/episode2/frame%d.txt", i); // sprintf pour formatter du texte dans un string
        Grille_histoire* gh = Grille_charger_fichier_H(nomfichier);

        usleep(500000); // 0.5s

        Grille_redessiner_H(gh);

        Grille_desallouer_H(&gh);
    }

    usleep(2000000); // 2s

    imprimer_texte_avec_animation("\033[1EIl parvint à sauver le bloc rose.\033[1E");

    usleep(1500000); // 1.5s

    for(int i=22; i<=25; i++){
        char nomfichier[100];
        sprintf(nomfichier, "./episodes/episode2/frame%d.txt", i); // sprintf pour formatter du texte dans un string
        Grille_histoire* gh = Grille_charger_fichier_H(nomfichier);

        usleep(500000); // 0.5s

        Grille_redessiner_H(gh);

        Grille_desallouer_H(&gh);
    }

    Grille_histoire* gh = Grille_charger_fichier_H("./episodes/episode2/frame25.txt"); // on reouvre pour la fin

    usleep(2000000); // 1s
    
    imprimer_texte_avec_animation("\nIls vécurent alors heureux pour toujours.\033[E1");

    for(int i=0; i<gh->n; i++){
        for(int j=0; j<gh->m; j++){
            gh->grille[i][j] = RIEN_H; // pour la fin de l'animation, on fait tout l'ecran noir
            usleep(12500); // 0.0125s
            Grille_redessiner_H(gh);
        }
    }

    Grille_desallouer_H(&gh);

    usleep(2000000);

    printf(
        "██╗      █████╗     ███████╗██╗███╗   ██╗\033[1E"
        "██║     ██╔══██╗    ██╔════╝██║████╗  ██║\033[1E"
        "██║     ███████║    █████╗  ██║██╔██╗ ██║\033[1E"
        "██║     ██╔══██║    ██╔══╝  ██║██║╚██╗██║\033[1E"
        "███████╗██║  ██║    ██║     ██║██║ ╚████║\033[1E"
        "╚══════╝╚═╝  ╚      ╚═╝     ╚═╝╚═╝  ╚═══╝\033[1E"
    );
    endwin();
    fflush(stdout);
}
