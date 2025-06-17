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
#include <assert.h>

/* Si vuole risolvere il gioco passo passo, sfruttando una ricerda in ampiezza
   BFS, osservando ogni possibile mossa possibile per poi ricostruirne la sequenza
   vincente una volta trovata la prima configurazione che rispetta tali condizioni
   di vittoria. */

/* Dimensiodi della griglia di gioco e numero di celle. */
#define ROWS 3
#define COLS 3
#define CELLS_NUMBER (ROWS * COLS)

/* File di inizializzazione della griglia */
const char *init_file_name = "test1.in";

/* Strutture dati per la soluzione della ricerca per ampiezza. */
/* Nodo del grafo degli stati di gioco. */
typedef struct
{
    int grid_state[ROWS][COLS];
    int move; /* Mossa che ha dato la struttura corrente. */
    struct TreeNode *father;
} TreeNode;

/* Nodo della coda di ricerca BFS */
typedef struct ListNode {
    TreeNode *val;
    struct ListNode *succ, *pred;
} ListNode;

/* Coda per BFS */
typedef struct {
    int length;
    ListNode *sentinel;
} List;

/* Listacontenente le posizioni delle stelle trovate, ovvero tutte le mosse possibili. */
typedef struct {
    int *position;
    int dimension;
    int capacity;
} starList;

/* Griglia di gioco. */
int starting_grid[ROWS][COLS];

/* Indicatore del contenuto di una cella,
   0 se questa è un buco nero, 1 se è una stella. */
enum
{
    BLACK_HOLE,
    STAR
};

/* Indica il risultato di una mossa. */
typedef enum
{
    OK,
    WON,
    LOST
} Move;

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
     {0, 1, 1}}
};

/* Funzioni per la gestione delle liste. */
/* Crea un nuovo nodo della lista. */
static ListNode *list_new_node(TreeNode *v)
{
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    assert(new_node != NULL);
    new_node->val = v;
    new_node->succ = new_node->pred = new_node;
    return new_node;
}

/* Crea una nuova lista (vuota). */
List *list_create(void)
{
    List *L = (List *)malloc(sizeof(List));
    assert(L != NULL);
    L->length = 0;
    L->sentinel = list_new_node(NULL); /* il valore contenuto nel nodo sentinella è irrilevante */
    return L;
}

/* Restituisce l'indirizzo di memoria della sentinella di `L`. */
ListNode *list_end(const List *L)
{
    assert(L != NULL);
    return L->sentinel;
}

/* Restituisce il nodo della lista successivo a quello fornito. */
ListNode *list_succ(const ListNode *n)
{
    assert(n != NULL);

    return n->succ;
}

/* Restituisce il nodo della lista precedente a quello fornito. */
ListNode *list_pred(const ListNode *n)
{
    assert(n != NULL);

    return n->pred;
}

/* Svuota lista e libera la memoria occupata dai suoi elementi. */
void list_clear(List *L)
{
    ListNode *node;
    assert(L != NULL);
    node = L->sentinel->succ;
    while (node != L->sentinel)
    {
        ListNode *succ = node->succ;
        free(node);
        node = succ;
    }
    L->length = 0;
    L->sentinel->pred = L->sentinel->succ = L->sentinel;
}

/* Libera la lista e libera la memoria da essa occupata. */
void list_destroy(List *L)
{
    assert(L != NULL);
    list_clear(L);
    free(L->sentinel);
    free(L);
}

/* Concatena due nodi `pred` e `succ`: `succ` diventa il successore di
   `pred`. */
static void list_join(ListNode *pred, ListNode *succ)
{
    assert(pred != NULL);
    assert(succ != NULL);

    pred->succ = succ;
    succ->pred = pred;
}

/* Crea un nuovo nodo contenente k, e lo inserisce immediatamente dopo
   il nodo n di L */
static void list_insert_after(List *L, ListNode *n, TreeNode *k)
{
    ListNode *new_node, *succ_of_n;
    assert(L != NULL);
    assert(n != NULL);

#if 1
    new_node = list_new_node(k);
    succ_of_n = list_succ(n);
    list_join(n, new_node);
    list_join(new_node, succ_of_n);
#else
    /* volendo realizzare questa funzione totalmente "a mano", senza
       usare altre funzioni di utilità definite in questo file, si può
       procedere come segue. */
    succ_of_n = n->succ;
    new_node = (ListNode*)malloc(sizeof(ListNode));
    assert(new_node != NULL);
    new_node->val = k;
    new_node->pred = n;
    new_node->succ = succ_of_n;
    n->succ = new_node;
    succ_of_n->pred = new_node;
#endif
    L->length++;
}

/* Inserisce un nuovo nodo contenente k alla fine della lista */
void list_add_last(List *L, TreeNode *k)
{
    assert(L != NULL);
    list_insert_after(L, L->sentinel->pred, k);
}

/* Rimuove il nodo n dalla lista L */
void list_remove(List *L, ListNode *n)
{
    assert(L != NULL);
    assert(n != NULL);
    assert(n != list_end(L));
    list_join(list_pred(n), list_succ(n));
    free(n);
    L->length--;
}

ListNode *list_first(const List *L)
{
    assert(L != NULL);

    return L->sentinel->succ;
}

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
            if (grid[i][j] == STAR)
                return 0;
        }
    }

    return 1;
}



/* Inverte il contenuto della cella alle coordinate [i][j] nella
   griglia passata. */
void invert(int i, int j)
{
    assert(i >= 0 && i < ROWS);
    assert(j >= 0 && j < COLS);
}

/* Shoot the specified cell of the game grid. */
int shoot(int k)
{
    
}

/* Fa esplodere la stella nella posizione k della griglia passata. */

/* Ritorna la posizione delle stelle nella configurazione passata. */
/*TODO*/

/* Metodo per la ricerca della combinazione di mosse più corta per
   l'ottenimento di una combinazione vincente. */
int BFS(TreeNode *root)
{
    /* Crea */
    List *queue = list_create();

    /*append first node*/
    list_add_last(queue, root);

    while (queue->length > 0)
    {
        ListNode *current_node = list_first(queue);
        TreeNode *current_tree_node = current_node->val;

        if(check_win(current_tree_node->grid_state))
        {
            while(current_tree_node->father != NULL)
            {
                printf("%d ", current_tree_node->move);
                current_tree_node = current_tree_node->father;
            }

            return 1; /* Found winnig path. */
        }
        else if(check_defeat(current_tree_node->grid_state))
        {
            list_remove(queue, current_node);
        }
        else
        {
            
        }
    }
    
    return -1; /* No winning path found. */
    /*cycle nodes*/
    /*check if node is winning*/
    /*if it is winning find all fathers recoursively and print each move*/
    /*if losing dequeue and continue*/
    /*if neither find all stars*/
    /*explode each and create a new node for each move and append*/
    /*dequeue this node and continue*/

    /*if it ends without wnning return -1*/
}

int main(int argc, char **argv)
{
    printf(argc);

    init_grid();

    print_board(starting_grid);

    return EXIT_SUCCESS;
}