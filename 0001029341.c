/*
 * Autore: Matteo Sartini
 * Matricola: 0001029341
 * Email: matteo.sartini8@studio.unibo.it
 * Classe B
 */

/* TODO: togliere, comando per compilazione
gcc -std=c90 -Wall -Wpedantic 0001029341.c -o 000102941 -lm
*/

#include <stdio.h>
#include <stdlib.h>

/* Dimensiodi della griglia di gioco e numero di celle. */
#define ROWS 3
#define COLS 3
#define CELLS_NUMBER (ROWS * COLS)

/* Griglia di gioco. */
int grid[ROWS][COLS];

/* File di inizializzazione della griglia */
const char *init_file_name = "test1.in";

/* Indicatore del contenuto di una cella,
   0 se questa è un buco nero, 1 se è una stella. */
enum
{
    BLACK_HOLE,
    STAR
};

/* Vicini di ogni cella, 1 se la casella
   è vicina 0 altrimenti. */
int neighbours[CELLS_NUMBER][ROWS][COLS] = {
    {{1, 1, 0}, /* cella 0 con il suo vicinato	*/
     {1, 1, 0},
     {0, 0, 0}},

    {{1, 1, 1}, /* cella 1 con il suo vicinato 	*/
     {0, 0, 0},
     {0, 0, 0}},

    {{0, 1, 1}, /* cella 2 con il suo vicinato 	*/
     {0, 1, 1},
     {0, 0, 0}},

    {{1, 0, 0}, /* cella 3 con il suo vicinato 	*/
     {1, 0, 0},
     {1, 0, 0}},

    {{0, 1, 0}, /* cella 4 con il suo vicinato 	*/
     {1, 1, 1},
     {0, 1, 0}},

    {{0, 0, 1}, /* cella 5 con il suo vicinato 	*/
     {0, 0, 1},
     {0, 0, 1}},

    {{0, 0, 0}, /* cella 6 con il suo vicinato 	*/
     {1, 1, 0},
     {1, 1, 0}},

    {{0, 0, 0}, /* cella 7 con il suo vicinato 	*/
     {0, 0, 0},
     {1, 1, 1}},

    {{0, 0, 0}, /* cella 8 con il suo vicinato 	*/
     {0, 1, 1},
     {0, 1, 1}}};

/* Inizializza la griglia secondo quanto specificato
   nel file di inizializzazione. */
void init_grid()
{
    FILE *init_file = fopen(init_file_name, "r");
    int i, j;
    char input;

    if (init_file == NULL)
    {
        printf("Errore nell'apertura del file.");
    }
    else
    {
        for (i = 0; i < ROWS; i++)
        {
            for (j = 0; j < COLS; j++)
            {
                do
                {
                    input = fgetc(init_file);
                } while (input == '\n');

                if (input == '.')
                    grid[i][j] = 0;
                else if (input == '*')
                    grid[i][j] = 1;
            }
        }
    }

    fclose(init_file);
}

/*TODO TOGLIERE
 */
void print_board(void)
{
    int i, j, idx = 0;
    printf("\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (grid[i][j] == BLACK_HOLE)
            {
                printf(". ");
            }
            else
            {
                printf("%d ", idx);
            }
            idx++;
        }
        printf("\n\n");
    }
    printf("\n");
}

int main(void)
{
    printf("Hello\n");

    init_grid();

    print_board();

    return EXIT_SUCCESS;
}