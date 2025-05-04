#ifndef HISTOIRE_H
#define HISTOIRE_H

// grille

typedef enum {RIEN_H, MUR_H, PION_BLEU_H, PION_ROSE_H, PION_MECHANT_H} element_grille;

typedef struct{
    int n, m;
    element_grille** grille; // matrice d'elements de grille
} Grille_histoire; 

Grille_histoire* Grille_initialiser_H(int n, int m); // initialise une grille
void Grille_desallouer_H(Grille_histoire** g); // desaloue une grille
void Grille_redessiner_H(Grille_histoire* g); // imprime a l'ecran la grille
Grille_histoire* Grille_charger_fichier_H(char* nom_fic); // cree et aloue une grille a partir d'un fichier modele

// pion

typedef enum {HAUT_H, BAS_H, DROITE_H, GAUCHE_H} evenement_pion_H; // renommes avec '_H' pour pas avoir de conflit avec les HAUT, BAS, etc.

typedef struct{
    int x, y; // coordonnees courantes
    int x_precedent, y_precedent; // coordonnees precedentes
} Pion_histoire;

Pion_histoire* allouer_pion_H();
void desallouer_pion_H(Pion_histoire** p);
void bouger_pion_logic_H (Grille_histoire* g, Pion_histoire* p, int a, int b); // fonction auxiliare de deplacer_pion
void deplacer_pion_H(Grille_histoire* g, Pion_histoire* p, evenement_pion_H even); // l'interface agreable pour celui qui programme, en utilisant evenement_pion

// ui (on l'utilise que dans histoire.c)

void imprimer_texte_avec_animation(const char* str);

// episodes

void episode1();
void episode2();

#endif