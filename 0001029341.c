/*
 * Autore: Matteo Sartini
 * Matricola: 0001029341
 * Email: matteo.sartini8@studio.unibo.it
 * Classe B
 */

/*BFS?*/

/* Griglia di gioco. */
int grid[3][3];

/* Indicatore del contenuto di una cella,
   0 se questa è un buco nero, 1 se è una stella. */
enum{ BLACK_HOLE, STAR };

/* Vicini di ogni cella, 1 se la casella
   è vicina 0 altrimenti. */
int vicini[9][3][3] = {
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
