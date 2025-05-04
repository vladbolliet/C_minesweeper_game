#ifndef GRILLE_H
#define GRILLE_H

#define FALSE 0
#define TRUE 1

typedef enum {RIEN, BOMBE, NOMBRE} type_cellule;

typedef struct {
    int est_revelee; 
    type_cellule type;
    int nb_bombes_voisignage;
    int pion_est_dessus;
    int est_marque; // si il est marque par un drapeau rouge
} Cellule;

typedef struct {
    Cellule** cases;
    int nb_lignes;
    int nb_colonnes;
 } Grille; 

Grille* creer_grille(int nb_lignes, int nb_colonnes); // creer une grille
void imprimer_grille(Grille* g); // imprimer la grille
int cellule_pas_entournee_par_bombes(Grille* g, int ri, int rj); // quand on genere les bombes, verifie si la cellule ou on veut placer une bombe est deja entourne par des bombes
int pas_sur_case_de_depart(Grille* g, int ri, int rj); // quand on genere les bombes, verifie si la cellule ou on veut placer une bombe est sur la case de depart de 3x3
int calculer_nombre_cellule(Grille* g, int a, int b); // calcule combien de bombes il y a autour de cette cellule
int nb_bombes_marquees(Grille* g); 
int nb_cases_non_revelees(Grille* g); // si nb_cases_non_relevees == nb_bombes alors le jeu se finit
void desallouer_grille(Grille** g);

#endif