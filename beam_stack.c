#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_FILE "matrix.in"
#define MAX_NODES 100
#define BEAM_WIDTH 3
#define MAX_STACK 1000

typedef struct {
    int nodes[MAX_NODES];
    int length;
} Path;

Path stack[MAX_STACK];
int top = -1;

int **create_adj_matrix(int n){
    int **matrix = (int **)malloc(sizeof(int *) * n);

    for(int i = 0; i < n; i++){
        matrix[i] = (int *)malloc(sizeof(int) *n);
        for(int j = 0; j < n; j++){
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

void create_edge(int **matrix, int v1, int v2){
    matrix[v1][v2] = 1;
    matrix[v2][v1] = 1;
}

Path beam_search(int **matrix, int n, int start) {
    typedef struct {
        int nodes[MAX_NODES]; //sequence of nodes
        int visited[MAX_NODES]; //cycle checks
        int length; //current path length
    } BeamPath;

    BeamPath beam[BEAM_WIDTH];
    int beamSize = 1;

    //initialize visited
    for (int i = 0; i < n; i++) beam[0].visited[i] = 0;
    beam[0].nodes[0] = start;
    beam[0].visited[start] = 1;
    beam[0].length = 1;

    Path best;
    best.length = 1;
    best.nodes[0] = start;

    //main loop
    //expand current beam and keep top candidates
    while (beamSize > 0) {
        //keep track of candidates
        BeamPath candidates[MAX_STACK];
        int candSize = 0;

        //expand current beam
        //visit all unvisited neighbors
        for (int i = 0; i < beamSize; i++) {
            BeamPath curr = beam[i];
            int last = curr.nodes[curr.length - 1];

            for (int j = 0; j < n; j++) {
                if (matrix[last][j] && !curr.visited[j]) {
                    BeamPath next = curr;

                    next.nodes[next.length++] = j;
                    next.visited[j] = 1;

                    candidates[candSize++] = next;
                }
            }
        }

        //break if no candidates are found (cannot be expanded)
        if (candSize == 0) break;

        //heuristic: fewer future moves
        //avoids dead ends
        //low score = fewer future moves/expansions = more contsrained
        int score[MAX_STACK];
        for (int i = 0; i < candSize; i++) {
            int last = candidates[i].nodes[candidates[i].length - 1];
            int future = 0;

            for (int j = 0; j < n; j++) {
                if (matrix[last][j] && !candidates[i].visited[j]) {
                    future++;
                }
            }

            score[i] = future;
        }

        //select top BEAM_WIDTH candidates
        //removed full sorting (partial selection instead)
        BeamPath newBeam[BEAM_WIDTH];
        int newSize = 0;

        for (int k = 0; k < BEAM_WIDTH; k++) {
            int bestCands = -1;

            for (int i = 0; i < candSize; i++) {
                if (score[i] == -1) continue;

                if (bestCands == -1 || score[i] < score[bestCands]) {
                    bestCands = i;
                }
            }

            //no more valid candidates
            if (bestCands == -1) break;

            newBeam[newSize++] = candidates[bestCands];
            score[bestCands] = -1; //mark used
        }

        //update best path found
        for (int i = 0; i < newSize; i++) {
            if (newBeam[i].length > best.length) {
                best.length = newBeam[i].length;
                for (int j = 0; j < best.length; j++) {
                    best.nodes[j] = newBeam[i].nodes[j];
                }
            }
        }

        //move to next beam
        for (int i = 0; i < newSize; i++) {
            beam[i] = newBeam[i];
        }
        beamSize = newSize;
    }

    return best;
}

int main() {
    // open file
    FILE *f = fopen(INPUT_FILE, "r");
    if (f == NULL) {
        printf("Error: could not open %s\n", INPUT_FILE);
        return 1;
    }

    // read number of nodes and edges
    int n, e;
    fscanf(f, "%d", &n);
    fscanf(f, "%d", &e);

    // create matrix and fill edges
    int **matrix = create_adj_matrix(n);
    for (int i = 0; i < e; i++) {
        int v1, v2;
        fscanf(f, "%d %d", &v1, &v2);
        create_edge(matrix, v1, v2);
    }
    fclose(f);

    // start timer
    clock_t start = clock();

    // run from every node, keep the best
    Path best;
    best.length = 0;
    for (int i = 0; i < n; i++) {
        Path p = beam_search(matrix, n, i);
        if (p.length > best.length) {
            best = p;
        }
    }

    // stop timer
    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    // print result
    printf("Longest path: ");
    for (int i = 0; i < best.length; i++) {
        if (i < best.length - 1)
            printf("%d -> ", best.nodes[i]);
        else
            printf("%d", best.nodes[i]);
    }
    printf("\nLength: %d edges\n", best.length - 1);
    printf("Time taken: %.6f seconds\n", time_taken);

    // free matrix
    for (int i = 0; i < n; i++) free(matrix[i]);
    free(matrix);

    return 0;
}

