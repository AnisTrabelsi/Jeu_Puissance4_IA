#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#define RESET_COLOR "\x1b[0m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"

typedef struct _tabPuissance4
{
    char **grille;
    uint32_t largeur;
    uint32_t hauteur;
} tabPuissance4;


/**************************************************************************/
tabPuissance4 *new_tabPuissance4(uint32_t largeur, uint32_t hauteur)
{
    tabPuissance4 *tp4 = malloc(sizeof(tabPuissance4));
    assert(tp4 != NULL);

    tp4->grille = malloc(largeur * sizeof(char *));
    assert(tp4->grille != NULL);

    for (uint32_t i = 0; i < largeur; i++)
    {
        tp4->grille[i] = calloc(hauteur, sizeof(char));
        assert(tp4->grille[i] != NULL);

        // Initialiser chaque élément de la colonne avec 'n'
        for (uint32_t j = 0; j < hauteur; j++)
        {
            tp4->grille[i][j] = 'n';
        }
    }

    tp4->largeur = largeur;
    tp4->hauteur = hauteur;

    return tp4;
}
/**************************************************************************/
void free_tabPuissance4(tabPuissance4 *tp4)
{
    for (uint32_t i = 0; i < tp4->largeur; i++)
    {
        free(tp4->grille[i]);
    }

    free(tp4->grille);
    free(tp4);
}

void afficher_grille(tabPuissance4 *tp4)
{
    int32_t j;
    uint32_t i;
    for (j = tp4->hauteur - 1; j >= 0; j--)
    {
        for (i = 0; i < tp4->largeur; i++)
        {
            if (tp4->grille[i][j] == 'n')
                printf("|   "); // Affiche une case vide
            else if (tp4->grille[i][j] == 'R')
                printf("| " RED "o" RESET_COLOR " "); // Affiche un rond rouge
            else if (tp4->grille[i][j] == 'J')
                printf("| " YELLOW "o" RESET_COLOR " "); // Affiche un rond jaune
        }
        printf("|\n");
    }

    // Affiche le numéro de chaque colonne
    for (i = 0; i < tp4->largeur; i++)
    {
        printf(" ---");
    }
    printf("\n");

    for (i = 0; i < tp4->largeur; i++)
    {
        printf("  %d ", i);
    }
    printf("\n\n");
}
/***********************************************************************************/
void add_dans_grille(tabPuissance4 *tp4, uint32_t indice_colonne, char c)
{
    if (indice_colonne >= tp4->largeur)
    {
        fprintf(stderr, "Indice de colonne invalide.\n");
        return;
    }

    int32_t j = tp4->hauteur - 1;

    while (j >= 0 && tp4->grille[indice_colonne][j] == 'n')
    {
        j--;
    }

    if (j >= 0)
    {
        tp4->grille[indice_colonne][j + 1] = c;
    }
    else
    {
        tp4->grille[indice_colonne][0] = c;
    }
}
/***************************************************************************************************/
_Bool canPlay(tabPuissance4 *tp4, uint32_t colonne)
{
    // Vérifier si la colonne spécifiée est dans les limites de la grille
    if (colonne >= tp4->largeur)
    {
        return 0; // Retourner false si la colonne est hors limites
    }

    // Vérifier si la première case (tête) de la colonne est vide ('n')
    if (tp4->grille[colonne][tp4->hauteur - 1] == 'n')
    {
        return 1; // Il y a de la place pour jouer, donc retourner true
    }

    return 0; // La colonne est pleine, retourner false
}
/********************************************************************************************/
_Bool grille_est_pleine(tabPuissance4 *tp4)
{
    for (uint32_t i = 0; i < tp4->largeur; i++)
    {
        if (tp4->grille[i][tp4->hauteur - 1] == 'n')
        {
            return 0; // La grille n'est pas pleine, il y a une cellule vide dans la dernière ligne.
        }
    }
    return 1; // La grille est pleine, pas de cellules vides dans la dernière ligne.
}
/***************************************************************************************/
_Bool verif_verticale(tabPuissance4 *tp4, uint32_t indice_colonne)
{
    int32_t j = tp4->hauteur - 1;

    while (j > 0 && tp4->grille[indice_colonne][j] == 'n')
    {
        j--;
    }

    char couleur_cherchee = tp4->grille[indice_colonne][j];
    uint32_t cpt_cases_contigues_verticales = 0;

    for (int32_t i = tp4->hauteur - 1; i >= 0; i--)
    {
        if (tp4->grille[indice_colonne][i] == couleur_cherchee)
        {
            cpt_cases_contigues_verticales++;
            if (cpt_cases_contigues_verticales >= 4)
                return 1;
        }
        else
        {
            cpt_cases_contigues_verticales = 0;
        }
    }

    return 0;
}
/**********************************************************************************/
_Bool verif_horizontale(tabPuissance4 *tp4, uint32_t indice_colonne)
{
    int32_t j = tp4->hauteur - 1;

    while (j > 0 && tp4->grille[indice_colonne][j] == 'n')
    {
        j--;
    }

    char couleur_cherchee = tp4->grille[indice_colonne][j];
    uint32_t cpt_cases_contigues_horizontales = 0;

    for (uint32_t i = 0; i < tp4->largeur; i++)
    {
        if (tp4->grille[i][j] == couleur_cherchee )
        {
            cpt_cases_contigues_horizontales++;
            if (cpt_cases_contigues_horizontales >= 4)
                return 1;
        }
        else
        {
            cpt_cases_contigues_horizontales = 0;
        }
    }

    return 0;
}
/***********************************************************************************/
_Bool verif_diagonale(tabPuissance4 *tp4, uint32_t indice_colonne)
{
    uint32_t ligne_ajoutee = tp4->hauteur - 1, col_min, col_max;

    // Trouver la hauteur de la dernière pièce ajoutée
    char couleur_cherchee;

    while (ligne_ajoutee > 0 && tp4->grille[indice_colonne][ligne_ajoutee] == 'n')
    {
        ligne_ajoutee--;
    }

    couleur_cherchee = tp4->grille[indice_colonne][ligne_ajoutee];

    if (indice_colonne >= 3)
    {
        col_min = (indice_colonne - 3 > 0) ? indice_colonne - 3 : 0;
    }
    else
    {
        col_min = 0;
    }

    if (indice_colonne + 3 < tp4->largeur)
    {
        col_max = (indice_colonne + 3 < tp4->largeur) ? indice_colonne + 3 : tp4->largeur - 1;
    }
    else
    {
        col_max = tp4->largeur - 1;
    }

    uint32_t k, j;
    uint32_t cpt1 = 1, cpt2 = 1, cpt3 = 1, cpt4 = 1;

    // diagonale haut gauche
    k = indice_colonne;
    j = ligne_ajoutee;
    while (k > col_min && j > 0)
    {
        k--;
        j--;
        if (tp4->grille[k][j] == couleur_cherchee)
        {
            cpt1++;
        }
        else
        {
            break;
        }
    }

    // diagonale bas droite
    k = indice_colonne;
    j = ligne_ajoutee;
    while (k < col_max && j < tp4->hauteur - 1)
    {
        k++;
        j++;
        if (tp4->grille[k][j] == couleur_cherchee)
        {
            cpt2++;
        }
        else
        {
            break;
        }
    }

    // diagonale haut droite
    k = indice_colonne;
    j = ligne_ajoutee;
    while (k > col_min && j < tp4->hauteur - 1)
    {
        k--;
        j++;
        if (tp4->grille[k][j] == couleur_cherchee)
        {
            cpt3++;
        }
        else
        {
            break;
        }
    }

    // diagonale bas gauche
    k = indice_colonne;
    j = ligne_ajoutee;
    while (k < col_max && j > 0)
    {
        k++;
        j--;
        if (tp4->grille[k][j] == couleur_cherchee)
        {
            cpt4++;
        }
        else
        {
            break;
        }
    }

    return (cpt1 >= 4 || cpt2 >= 4 || cpt3 >= 4 || cpt4 >= 4 || (cpt1 + cpt2 - 1) >= 4 || (cpt3 + cpt4 - 1) >= 4);
}
/***********************************************************************************/
int main()
{
    uint32_t largeur, hauteur;
    printf("Donner la largeur de la grille:   \n");
    scanf("%u", &largeur);
    printf("Donner la hauteur de la grille:  \n");
    scanf("%u", &hauteur);

    tabPuissance4 *grille = new_tabPuissance4(largeur, hauteur);
    _Bool finDuJeu = 0;
    char joueurActuel = 'R'; // R pour Rouge, J pour Jaune

    while (!finDuJeu)
    {
        uint32_t colonne;

        do
        {
            printf("Joueur %c, choisissez une colonne (0-%u) : ", joueurActuel, largeur - 1);
            scanf("%u", &colonne);
        } while (colonne >= largeur || colonne < 0 || !canPlay(grille, colonne));

        // Ajout du jeton dans la grille
        add_dans_grille(grille, colonne, joueurActuel);

        // Affichage de la grille
        afficher_grille(grille);

        if (!grille_est_pleine(grille))
        { // Vérification des conditions de victoire
            if (verif_verticale(grille, colonne) || verif_horizontale(grille, colonne) || verif_diagonale(grille, colonne))
            {
                printf("Le joueur %c gagne !\n", joueurActuel);
                finDuJeu = 1;
            }
            else
            // Changement de joueur
            {
                if (joueurActuel == 'R')
                {
                    joueurActuel = 'J';
                }
                else
                {
                    joueurActuel = 'R';
                }
            }
        }
        else
        {
            printf("Egalite\n");
            finDuJeu = 1;
        }
    }

    free_tabPuissance4(grille);

    return 0;
}
