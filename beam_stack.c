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

void push(Path p){
    if(top < MAX_STACK - 1){
        top++;
        stack[top] = p;
    }
}

Path pop(){
    if (top == -1) {
        printf("Stack underflow\n");
        exit(1);
    }
    Path p = stack[top];
    top--;
    return p;
}

int isEmpty(){
    return (top == -1);
}

int contains(Path p, int node){
    for(int i = 0; i < p.length; i++){
        if (p.nodes[i] == node){
            return 1;
        }
               
    }
    return 0;
}

Path extend(Path p, int node){
    Path newP = p;
    newP.nodes[newP.length] = node;
    newP.length++;
    return newP;
}

Path beam_stack_search(int **matrix, int n, int start) {

    Path currentBeam[BEAM_WIDTH];
    int beamSize = 1;

    // initialize starting path
    currentBeam[0].nodes[0] = start;
    currentBeam[0].length = 1;

    Path bestPath = currentBeam[0];

    while (beamSize > 0 || !isEmpty()) {

        // backtrack if needed
        if (beamSize == 0) {
            currentBeam[0] = pop();
            beamSize = 1;
            continue;
        }

        Path nextBeam[MAX_STACK];
        int nextSize = 0;

        // expand beam
        for (int i = 0; i < beamSize; i++) {
            Path curr = currentBeam[i];
            int last = curr.nodes[curr.length - 1];

            for (int j = 0; j < n; j++) {
                if (matrix[last][j] == 1 && !contains(curr, j)) {
                    Path newPath = extend(curr, j);

                    if (nextSize < MAX_STACK) {
                        nextBeam[nextSize++] = newPath;
                    }
                }
            }
        }

        // no expansion so we need to backtrack
        if (nextSize == 0) {
            if (!isEmpty()) {
                currentBeam[0] = pop();
                beamSize = 1;
            } else {
                break;
            }
            continue;
        }
        for (int i = 0; i < nextSize - 1; i++) {
            for (int j = i + 1; j < nextSize; j++) {
                if (nextBeam[j].length > nextBeam[i].length) {
                    Path temp = nextBeam[i];
                    nextBeam[i] = nextBeam[j];
                    nextBeam[j] = temp;
                }
            }
        }

        // compute min(nextSize, BEAM_WIDTH) so it won't overlflow
        int newBeamSize;
        if (nextSize < BEAM_WIDTH) {
            newBeamSize = nextSize;
        } else {
            newBeamSize = BEAM_WIDTH;
        }

        // push discarded paths to stack
        for (int i = newBeamSize; i < nextSize; i++) {
            push(nextBeam[i]);
        }

        // update beam
        for (int i = 0; i < newBeamSize; i++) {
            currentBeam[i] = nextBeam[i];

            if (currentBeam[i].length > bestPath.length) {
                bestPath = currentBeam[i];
            }
        }

        beamSize = newBeamSize;
    }

    return bestPath;
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
        top = -1;  // reset stack for each start node
        Path p = beam_stack_search(matrix, n, i);
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

