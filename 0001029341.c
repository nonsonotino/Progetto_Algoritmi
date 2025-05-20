/*
 * Autore: Matteo Sartini
 * Matricola: 0001029341
 * Email: matteo.sartini8@studio.unibo.it
 * Classe B
 */

/* TODO: TOGLIERE, comando per compilazione
gcc -std=c90 -Wall -Wpedantic 0001029341.c -o 000102941 -lm
*/

#include <stdio.h>
#include <stdlib.h>

/* Si vuole risolvere il giovco passo passo, sfruttando una ricerda in ampiezza
   BFS, osservando ogni possibile mossa nel gioco per poi ricostruire la sequenza
   di mosse una volta trovata la prima configurazione vincente. */

/* Dimensiodi della griglia di gioco e numero di celle. */
#define ROWS 3
#define COLS 3
#define CELLS_NUMBER (ROWS * COLS)

/* Griglia di gioco. */
int starting_grid[ROWS][COLS];

/* File di inizializzazione della griglia */
const char *init_file_name = "test1.in";

/* Indicatore del contenuto di una cella,
   0 se questa è un buco nero, 1 se è una stella. */
enum
{
    BLACK_HOLE,
    STAR
};

/* Strutture dati per la soluzione della ricerca per ampiezza. */

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
        return EXIT_FAILURE;
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
                    starting_grid[i][j] = BLACK_HOLE;
                else if (input == '*')
                    starting_grid[i][j] = STAR;
            }
        }
    }

    fclose(init_file);
}

/*
    TODO TOGLIERE, metodo per il debug, stampa della matrice
*/
void print_board(int grid[ROWS][COLS])
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

/* Ritorna true (1) se la configurazione passata è vincente. */
int check_win(int grid[ROWS][COLS])
{
    static const int winning_grid[ROWS][COLS] = {{1, 1, 1},
                                                 {1, 0, 1},
                                                 {1, 1, 1}};
    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (grid[i][j] != winning_grid[i][j])
                return 0;
        }
    }

    return 1;
}

/* Ritorna true (1) se la configurazione passata è perdente. */
int check_defeat(int grid[ROWS][COLS])
{
    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if(grid[i][j] == STAR)
                return 0;
        }
    }

    return 1;
}

/* Ritorna la posizione delle stelle nella configurazione passata. */
/*TODO*/

/* Metodo per la ricerca della combinazione di mosse più corta per
   l'ottenimento di una combinazione vincente. */
void BFS(int grid[ROWS][COLS])
{
}

int main()
{
    printf("Hello\n");

    init_grid();

    print_board(starting_grid);

    return EXIT_SUCCESS;
}