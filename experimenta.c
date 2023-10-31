#include <stdio.h>
#include <limits.h>

#define V 6 // Número de vértices

void printSolution(int dist[][V]) {
    printf("Matriz de custo mínima:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INT_MAX){
                dist[i][j]=0;
                printf("%d\t", dist[i][j]);
            }else
                printf("%d\t", dist[i][j]);
        }
        printf("\n");
    }
}

void floydWarshall(int graph[][V]) {
    int dist[V][V];
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX
                    && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printSolution(dist);
}

int main() {
    int graph[V][V] = {
     {0, 2, INT_MAX, 5, INT_MAX, INT_MAX},
        {INT_MAX, 0, INT_MAX, INT_MAX, INT_MAX, INT_MAX},
        {INT_MAX, 2, 0, INT_MAX, INT_MAX, 5},
        {INT_MAX, INT_MAX, INT_MAX, 0, 1, INT_MAX},
        {3, 9, 3, INT_MAX, 0, INT_MAX},
        {INT_MAX, INT_MAX, INT_MAX, INT_MAX, 1, 0}};

    floydWarshall(graph);

    return 0;
}
