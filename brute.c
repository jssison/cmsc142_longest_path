#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//modify input file here
#define INPUT_FILE "matrix.in"

int **create_adj_matrix(int n) {
    int **matrix = (int **)(malloc(sizeof(int *)*n));

    for (int i=0; i < n; i++) {
        matrix[i] = (int *)(malloc(sizeof(int)*n));
        for (int j=0; j < n; j++) {
            //initialize everything to 0
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

void create_edge(int **matrix, int v1, int v2) {
    matrix[v1][v2] = 1;
    matrix[v2][v1] = 1;
}

void dfs(int **matrix, int curr_v, int n, int *visited, int length, int *max_path,
    int *curr_path, int *longest_path) {

    //add current vertex to current path
    curr_path[length] = curr_v;

    if (length > *max_path) {
        //current path is longer than best path
        *max_path = length;

        //copy current path to longest path
        for (int i=0; i <= length; i++) {
            longest_path[i] = curr_path[i];
        }
    }

    for (int i=0; i < n; i++) {
        if (matrix[curr_v][i] == 1 && !visited[i]) {
            //mark as visited
            visited[i] = 1;
            //dfs from recently visited node
            dfs(matrix, i, n, visited, length+1, max_path, curr_path, longest_path);
            visited[i] = 0; //for backtracking
        }
    }
}

void main() {
    FILE *fp = fopen(INPUT_FILE, "r");
    if (fp == NULL) {
        printf("Error: Unable to read file.");
        return;
    }

    //first row, number of vertices
    int v, e;
    fscanf(fp, "%d", &v);

    //create adjacency matrix
    int **matrix = create_adj_matrix(v);

    //read number of edges
    fscanf(fp, "%d", &e);

    //read edges
    //since graph is undirected, both entries of the the vertices 
    //in the matrix will be marked 1
    for (int i=0; i < e; i++) {
        int v1, v2;
        fscanf(fp, "%d %d", &v1, &v2);
        create_edge(matrix, v1, v2);
    }

    //close file
    fclose(fp);

    int max_path = 0;
    int *visited = (int *)(malloc(sizeof(int)*v));
    int *curr_path = (int *)(malloc(sizeof(int)*v));
    int *longest_path = (int *)(malloc(sizeof(int)*v));

    //initialize longest path (just to be safe)
    for (int i=0; i < v; i++) {
        longest_path[i] = -1;
    }

    clock_t start, end;
    start = clock();

    for (int i=0; i < v; i++) {
        for (int j=0; j < v; j++) {
            //mark every vertex as unvisited
            visited[j] = 0;
        }

        //mark ith vertex as visited
        //starting vertex
        visited[i] = 1;
        //dfs from starting vertex
        dfs(matrix, i, v, visited, 0, &max_path, curr_path, longest_path);
    }

    end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;


    //print longest path
    for (int i=0; i <= max_path; i++) {
        if (i < max_path) {
            printf("%d -> ", longest_path[i]);
        }
        else{
            printf("%d", longest_path[i]);
        }
    }
    printf("\n");
    printf("Length: %d edges\n", max_path);
    printf("Time taken: %.6f seconds\n", time_taken);

    //free matrix and visited
    for (int i=0; i < v; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(visited);
    free(curr_path);
    free(longest_path);
}