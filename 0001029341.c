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

/* Strutture dati per la soluzione della ricerca per ampiezza. */
/* Nodo del grafo degli stati di gioco. */
typedef struct TreeNode
{
    int **grid_state;
    int move; /* Mossa che ha dato la struttura corrente. */
    struct TreeNode *father;
} TreeNode;

/* Nodo della coda di ricerca BFS */
typedef struct ListNode
{
    TreeNode *val;
    struct ListNode *succ, *pred;
} ListNode;

/* Coda per BFS */
typedef struct
{
    int length;
    ListNode *sentinel;
} List;

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
     {0, 1, 1}}};

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
    new_node = (ListNode *)malloc(sizeof(ListNode));
    assert(new_node != NULL);
    new_node->val = k;
    new_node->pred = n;
    new_node->succ = succ_of_n;
    n->succ = new_node;
    succ_of_n->pred = new_node;
#endif
    L->length++;
}

/* Inserisce un nuovo nodo contenente k alla fine della lista. */
void list_add_last(List *L, TreeNode *k)
{
    assert(L != NULL);
    list_insert_after(L, L->sentinel->pred, k);
}

/* Rimuove il nodo n dalla lista L. */
void list_remove(List *L, ListNode *n)
{
    assert(L != NULL);
    assert(n != NULL);
    assert(n != list_end(L));
    list_join(list_pred(n), list_succ(n));
    free(n);
    L->length--;
}

/* Ritorna il primo elemento della lista passata in input. */
ListNode *list_first(const List *L)
{
    assert(L != NULL);

    return L->sentinel->succ;
}

/* Alloca e ritona una nuova matrice 3x3. */
int **allocateMatrix()
{
    int i;
    int **new_matrix = (int **)malloc(sizeof(int *) * ROWS);

    for (i =0; i < ROWS; i++)
    {
        new_matrix[i] = (int *)malloc(sizeof(int) * COLS);
    }

    return new_matrix;
}

/* Copia il contenuto della matrice src nella matrice dest. */
int **copy_matrix(int** src)
{
    int i, j;
    int **dest = allocateMatrix();
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            dest[i][j] = src[i][j];
        }
    }

    return dest;
}


/* Inizializza la griglia secondo quanto specificato
   nel file di inizializzazione. */
int **init_grid(const char *init_file_name)
{
    FILE *init_file = fopen(init_file_name, "r");
    int **grid = allocateMatrix();
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
                    grid[i][j] = BLACK_HOLE;
                else if (input == '*')
                    grid[i][j] = STAR;
            }
        }
    }

    fclose(init_file);
    return grid;
}

/* Ritorna true (1) se la configurazione passata è vincente. */
int check_win(int **grid)
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
int check_defeat(int **grid)
{
    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (grid[i][j] == BLACK_HOLE)
                return 0;
        }
    }

    return 1;
}

/* Inverte il contenuto della cella alle coordinate [i][j] nella
   griglia passata. */
void swap(int i, int j, int **grid)
{
    assert(i >= 0 && i < ROWS);
    assert(j >= 0 && j < COLS);
    grid[i][j] = 1 - grid[i][j];
}

/* Spara sulla casella selezionata nella griglia passata. */
int **shoot(int k, int **grid)
{
    int **new_grid = copy_matrix(grid);
    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (neighbours[k][i][j])
            {
                swap(i, j, new_grid);
            }
        }
    }

    return new_grid;
}

/* Fa esplodere la stella nella posizione k della griglia passata. */

/* Ritorna la posizione delle stelle nella configurazione passata. */
int *find_stars(int **grid)
{
    /* Array dinamico contenente le posizioni delle stelle trovate. */
    int *stars = (int*)malloc(sizeof(int) * ROWS * COLS);

    /* Contatori di supporto. */
    int i, j, c = 0;

    /* Inizializzazione dell'array con valori non significativi. */
    for (i = 0; i < ROWS * COLS; i++)
    {
        stars[i] = -1;
    }

    /* Scansione della griglia per trovare le stelle. */
    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            if(grid[i][j] == STAR)
            {
                stars[c] = i * COLS + j; /* Calcola la posizione lineare */
                c += 1;
            }
        }
    }

    return stars;
}

/* Metodo per la ricerca della combinazione di mosse più corta per
   l'ottenimento di una combinazione vincente. */
int BFS(TreeNode *root)
{
    /* Crea lista. */
    List *queue = list_create();

    /*Inizializzazione variabili di supporto per lavorazione dati. */
    ListNode *current_node;
    TreeNode *current_tree_node;
    TreeNode *new_tree_node;
    int *stars;
    
    /* Inizializzazione contatori. */
    int i;

    list_add_last(queue, root);

    while (queue->length > 0)
    {
        current_node = list_first(queue);
        current_tree_node = current_node->val;

        if (check_win(current_tree_node->grid_state))
        {
            while (current_tree_node->father != NULL)
            {
                printf("%d\n", current_tree_node->move);
                current_tree_node = current_tree_node->father;
            }

            return 1; /* Found winnig path. */
        }
        else if (check_defeat(current_tree_node->grid_state))
        {
            list_remove(queue, current_node);
        }
        else
        {
            stars = find_stars(current_tree_node->grid_state);

            for(i = 0; stars[i] != -1; i++)
            {
                new_tree_node = (TreeNode *)malloc(sizeof(TreeNode));
                new_tree_node->father = current_tree_node;
                new_tree_node->move = stars[i];
                new_tree_node->grid_state = shoot(stars[i], current_tree_node->grid_state);

                list_add_last(queue, new_tree_node);
            }

            list_remove(queue, current_node);
        }
    }

    printf("%d", -1);

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
    /* Inizializzazione nodo radice dell'albero da esplorare*/
    TreeNode *root;

    /* Inizializzazione dello stato iniziale del gioco. */
    int **starting_grid = init_grid(argv[1]);

    root = (TreeNode *)malloc(sizeof(TreeNode));
    root->father = NULL;
    root->move = -1; /* La radice non ha una mossa associata. */
    root->grid_state = copy_matrix(starting_grid);

    BFS(root);

    return EXIT_SUCCESS;
}