#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#define RESET_COLOR "\x1b[0m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"

typedef struct _tabPuissance4
{
    char **grille;
    uint32_t largeur;
    uint32_t hauteur;
} tabPuissance4;

typedef struct Node
{
    tabPuissance4 *grille;
    struct Node **children;
    int numChildren;
    char currentPlayer;
    bool isTerminal; // Ajouté pour marquer si le nœud est terminal
} Node;

/// @brief ///////////////////////////////////////////////
/// @param largeur
/// @param hauteur
/// @return
tabPuissance4 *new_tabPuissance4(uint32_t largeur, uint32_t hauteur)
{
    uint32_t i, j;
    tabPuissance4 *tp4 = malloc(sizeof(tabPuissance4));
    assert(tp4 != NULL);

    tp4->grille = malloc(largeur * sizeof(char *));
    assert(tp4->grille != NULL);

    for (i = 0; i < largeur; i++)
    {
        tp4->grille[i] = calloc(hauteur, sizeof(char));
        assert(tp4->grille[i] != NULL);

        // Initialiser chaque élément de la colonne avec 'n'
        for (j = 0; j < hauteur; j++)
        {
            tp4->grille[i][j] = 'n';
        }
    }

    tp4->largeur = largeur;
    tp4->hauteur = hauteur;

    return tp4;
}
/**************************************************************************/
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
/**************************************************************/
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
/************************************************************************************/
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
/************************************************************************************/
_Bool grille_est_pleine(tabPuissance4 *tp4)
{
    uint32_t i;
    for (i = 0; i < tp4->largeur; i++)
    {
        if (tp4->grille[i][tp4->hauteur - 1] == 'n')
        {
            return 0; // La grille n'est pas pleine
        }
    }

    return 1; // La grille est pleine,
}
/************************************************************************************/
_Bool verif_verticale(tabPuissance4 *tp4, uint32_t indice_colonne)
{
    int32_t j = tp4->hauteur - 1;
    int32_t i;
    while (j > 0 && tp4->grille[indice_colonne][j] == 'n')
    {
        j--;
    }

    char couleur_cherchee = tp4->grille[indice_colonne][j];
    uint32_t cpt_cases_contigues_verticales = 0;

    for (i = tp4->hauteur - 1; i >= 0; i--)
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
/***********************************************************************************/

_Bool verif_horizontale(tabPuissance4 *tp4, uint32_t indice_colonne)
{
    int32_t j = tp4->hauteur - 1;
    uint32_t i;
    while (j > 0 && tp4->grille[indice_colonne][j] == 'n')
    {
        j--;
    }

    char couleur_cherchee = tp4->grille[indice_colonne][j];
    uint32_t cpt_cases_contigues_horizontales = 0;

    for (i = 0; i < tp4->largeur; i++)
    {
        if (tp4->grille[i][j] == couleur_cherchee)
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
/************************************************************************************/
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
/*****************************************************************************/

Node *createNode(tabPuissance4 *grille, int cols, char currentPlayer)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        // Gestion d'erreur en cas d'échec d'allocation
        return NULL;
    }
    newNode->grille = grille;
    newNode->children = malloc(cols * sizeof(Node *));
    if (!newNode->children)
    {
        // Gestion d'erreur
        free(newNode);
        return NULL;
    }

    newNode->numChildren = cols;
    newNode->currentPlayer = currentPlayer; // Définition du joueur actuel
    int32_t i;
    for (i = 0; i < cols; ++i)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}
/**************************************************************************/
void free_tabPuissance4(tabPuissance4 *tp4)
{
    uint32_t i;
    for (i = 0; i < tp4->largeur; i++)
    {
        free(tp4->grille[i]);
    }

    free(tp4->grille);
    free(tp4);
}
void freeTree(Node *node)
{
    int32_t i;
    if (node == NULL)
        return;

    for (i = 0; i < node->numChildren; ++i)
    {
        freeTree(node->children[i]);
        node->numChildren--; // Décrémenter le compteur après la libération de chaque enfant
    }

    free_tabPuissance4(node->grille);
    free(node->children);
    free(node);
}
/*******************************************************************************************/
int evaluateWindow(char *window)
{
    int score = 0;
    int i;
    char player = 'J';

    int playerCount = 0;
    int emptyCount = 0;

    for (i = 0; i < 4; i++)
    {
        if (window[i] == player)
        {
            playerCount++;
        }
        else if (window[i] == 'n')
        {
            emptyCount++;
        }
    }

    if (playerCount == 4)
    {
        score += 100;
    }
    else if (playerCount == 3 && emptyCount == 1)
    {
        score += 5;
    }
    else if (playerCount == 2 && emptyCount == 2)
    {
        score += 2;
    }

    if (playerCount == 0 && emptyCount != 4)
    {
        if (4 - emptyCount == 4)
        {
            score -= 95;
        }
        else if (4 - emptyCount == 3)
        {
            score -= 4;
        }
        else if (4 - emptyCount == 2)
        {
            score -= 1;
        }
    }

    return score;
}
/**********************************************************************************************/
int score(tabPuissance4 *tp4)
{
    int score = 0;
    uint32_t i, j, k;
    // Combinaisons horizontales
    if (tp4->largeur > 3)
    {
        for (i = 0; i < tp4->hauteur; i++)
        {
            char chaine[5] = {'\0'}; // La chaîne de 4 caractères + le caractère nul
            for (j = 0; j <= tp4->largeur - 4; j++)
            {
                for (k = 0; k < 4; k++)
                {
                    chaine[k] = tp4->grille[j + k][i];
                }
                score += evaluateWindow(chaine); // Évaluation après la complétion de la chaîne
            }
        }
    }

    // Combinaisons verticales
    if (tp4->hauteur > 3)
    {
        for (i = 0; i <= tp4->hauteur - 4; i++)
        {
            char chaine[5] = {'\0'};
            for (j = 0; j < tp4->largeur; j++)
            {
                for (k = 0; k < 4; k++)
                {
                    chaine[k] = tp4->grille[j][i + k];
                }
                score += evaluateWindow(chaine); // Évaluation après la complétion de la chaîne
            }
        }
    }

    // Combinaisons diagonales
    if (tp4->largeur > 3 && tp4->hauteur > 3)
    {
        char chaine1[5] = {'\0'};
        char chaine2[5] = {'\0'};
        for (i = 0; i <= tp4->hauteur - 4; i++)
        {
            for (j = 0; j <= tp4->largeur - 4; j++)
            {
                for (k = 0; k < 4; k++)
                {
                    chaine1[k] = tp4->grille[j + k][i + k];
                    chaine2[k] = tp4->grille[j + 3 - k][i + k];
                }
                score += evaluateWindow(chaine1); // Évaluation après la complétion de la chaîne
                score += evaluateWindow(chaine2); // Évaluation après la complétion de la chaîne
            }
        }
    }
    return score;
}
/*******************************************************************************************/
// Fonction pour copier une grille
tabPuissance4 *copygrille(const tabPuissance4 *original)
{
    // Allouer de la mémoire pour la nouvelle structure
    tabPuissance4 *copy = malloc(sizeof(tabPuissance4));
    if (copy == NULL)
    {
        // Gestion d'erreur : Échec de l'allocation mémoire
        exit(EXIT_FAILURE);
    }

    // Copier les informations générales de la grille
    copy->largeur = original->largeur;
    copy->hauteur = original->hauteur;

    // Allouer de la mémoire pour la nouvelle grille
    copy->grille = malloc(original->largeur * sizeof(char *));
    if (copy->grille == NULL)
    {
        // Gestion d'erreur : Échec de l'allocation mémoire
        free(copy); // Libérer la mémoire allouée pour la structure
        exit(EXIT_FAILURE);
    }
    uint32_t i, j;
    // Copier chaque ligne de la grille
    for (i = 0; i < original->largeur; i++)
    {
        copy->grille[i] = malloc(original->hauteur * sizeof(char));
        if (copy->grille[i] == NULL)
        {
            // Gestion d'erreur : Échec de l'allocation mémoire
            // Libérer la mémoire allouée pour la structure et les lignes précédentes
            for (j = 0; j < i; j++)
            {
                free(copy->grille[j]);
            }
            free(copy);
            exit(EXIT_FAILURE);
        }

        // Copier les éléments de chaque ligne de la grille
        memcpy(copy->grille[i], original->grille[i], original->hauteur);
    }

    return copy;
}
/************************************************************************************************/
void buildTree(Node *node, char currentPlayer, int cols, int rows, int depth)
{
    // Vérifier la condition d'arrêt basée sur la profondeur
    if (depth == 0)
    {
        node->isTerminal = true;
        return;
    }

    char nextPlayer;
    if (currentPlayer == 'J')
    {
        nextPlayer = 'R';
    }
    else
    {
        nextPlayer = 'J';
    }
    node->children = calloc(cols, sizeof(Node *)); // Initialiser les enfants

    for (int i = 0; i < cols; ++i)
    {
        if (canPlay(node->grille, i))
        {
            tabPuissance4 *newgrille = copygrille(node->grille);
            add_dans_grille(newgrille, i, currentPlayer);

            // Vérifier si le jeu se termine après ce coup
            if (verif_verticale(newgrille, i) ||
                verif_horizontale(newgrille, i) ||
                verif_diagonale(newgrille, i) ||
                grille_est_pleine(newgrille))
            {
                node->children[i] = createNode(newgrille, cols, currentPlayer); // Créer un nœud même si le jeu se termine
                node->children[i]->isTerminal = true;                           // Marquer le nœud comme terminal
            }
            else
            {
                // Créer un nœud enfant si le jeu ne se termine pas
                node->children[i] = createNode(newgrille, cols, nextPlayer);
                buildTree(node->children[i], nextPlayer, cols, rows, depth - 1);
            }
        }
        // Si canPlay est faux, la branche n'est pas développée
    }
}
/****************************************************************************************/
int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}
/**************************************************************************************/
// Fonction utilitaire pour obtenir le minimum de deux valeurs
int min(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}
/***************************************************************************************/
int minimax(Node *node, int depth, bool isMaxPlayer, int lignes, int colonnes)
{
    // Vérification si le nœud est terminal ou si la profondeur maximale est atteinte
    if (depth == 0 || node->isTerminal)
    {
        return score(node->grille); // Évaluer et retourner le score du nœud actuel
    }

    int value;
    if (isMaxPlayer)
    {
        value = INT_MIN;
        // Parcourir les enfants du nœud actuel
        for (int i = 0; i < node->numChildren; ++i)
        {
            if (node->children[i] != NULL)
            {
                // Choisir la valeur maximale
                value = max(value, minimax(node->children[i], depth - 1, false, lignes, colonnes));
            }
        }
    }
    else // Joueur minimisant
    {
        value = INT_MAX;
        // Parcourir les enfants du nœud actuel
        for (int i = 0; i < node->numChildren; ++i)
        {
            if (node->children[i] != NULL)
            {
                // Choisir la valeur minimale
                value = min(value, minimax(node->children[i], depth - 1, true, lignes, colonnes));
            }
        }
    }

    return value; // Retourner la meilleure évaluation pour le nœud actuel
}
/******************************************************************************************/
int getBestMove(Node *node, int depth, bool isMaxPlayer, int lignes, int colonnes)
{
    int bestMove = -1; // Initialiser avec une valeur non valide
    int bestEval;
    if (isMaxPlayer)
    {
        bestEval = INT_MIN;
    }
    else
    {
        bestEval = INT_MAX;
    }
    // Parcourir tous les coups possibles
    for (int i = 0; i < node->numChildren; ++i)
    {
        if (node->children[i] != NULL)
        {
            // Calculer la valeur pour le nœud enfant en appelant récursivement minimax
            int eval = minimax(node->children[i], depth - 1, !isMaxPlayer, lignes, colonnes);

            // Pour le joueur maximisant, choisir le coup avec la valeur maximale
            if (isMaxPlayer && eval > bestEval)
            {
                bestEval = eval;
                bestMove = i; // Mettre à jour le meilleur coup
            }
            // Pour le joueur minimisant, choisir le coup avec la valeur minimale
            else if (!isMaxPlayer && eval < bestEval)
            {
                bestEval = eval;
                bestMove = i; // Mettre à jour le meilleur coup
            }
        }
    }

    return bestMove; // Retourner le meilleur coup à jouer
}
/*********************************************************************************/
int main()
{
    uint32_t largeur, hauteur;
    printf("Donner la largeur de la grille:  \n");
    scanf("%u", &largeur);
    printf("Donner la hauteur de la grille:  \n");
    scanf("%u", &hauteur);

    tabPuissance4 *grille = new_tabPuissance4(largeur, hauteur);
    bool finDuJeu = 0;
    char joueurActuel = 'R'; // R pour Rouge, J pour Jaune

    while (!finDuJeu)
    {
        uint32_t colonne;
        if (!grille_est_pleine(grille))
        {
            if (joueurActuel == 'R')
            {
                do
                {
                    printf("Joueur 'R' , choisissez une colonne (0-%u) : ", largeur - 1);
                    scanf("%u", &colonne);
                } while (colonne >= largeur || colonne < 0 || !canPlay(grille, colonne));
            }
            add_dans_grille(grille, colonne, joueurActuel);
            afficher_grille(grille);

            if (verif_verticale(grille, colonne) || verif_horizontale(grille, colonne) || verif_diagonale(grille, colonne))
            {
                printf("Le joueur %c gagne !\n", joueurActuel);
                finDuJeu = 1;
            }

            else
            {
                clock_t start_time = clock();

                joueurActuel = 'J';
                Node *racine = createNode(grille, largeur, joueurActuel);
                buildTree(racine, joueurActuel, largeur, hauteur, 5);
                int meilleurCoup = getBestMove(racine, joueurActuel, largeur, hauteur, 5);
                printf("Meilleur coup IA: %d \n", meilleurCoup);
                add_dans_grille(grille, meilleurCoup, joueurActuel);
                afficher_grille(grille);
                clock_t end_time = clock();

                // Calcul du temps écoulé
                double cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

                // Affichez le temps écoulé
                printf("Le temps ecoule pour que l'IA joue son tour est %f secondes.\n", cpu_time_used);
                if (verif_verticale(grille, meilleurCoup) || verif_horizontale(grille, meilleurCoup) || verif_diagonale(grille, meilleurCoup))
                {
                    printf("Le joueur %c gagne !\n", joueurActuel);
                    finDuJeu = 1;
                }

                joueurActuel = 'R';
            }
        }
        else
        {
            printf("egalite \n");
            finDuJeu = 1;
        }
    }
    // Libérer la mémoire avant de quitter
    free_tabPuissance4(grille);

    return 0;
}