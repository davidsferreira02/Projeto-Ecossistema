#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define INF 99999

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int n, rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define o número de vértices do grafo (tamanho da matriz)
    n = 6;

    // Divide a matriz em submatrizes
    int submatrix_size = n / size;
    int submatrix[n][n];
    
    // Inicialize a submatriz com valores infinitos (exceto para a diagonal principal)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                submatrix[i][j] = 0;
            } else {
                submatrix[i][j] = INF;
            }
        }
    }
    
    // Leitura da matriz completa no processo 0 (apenas para fins de exemplo)
    if (rank == 0) {
        int matrix[6][6] = {
            {0, 2, INF, 5, INF, INF},
            {INF, 0, INF, INF, INF, INF},
            {INF, 2, 0, INF, INF, 5},
            {INF, INF, INF, 0, 1, INF},
            {3, 9, 3, INF, 0, INF},
            {INF, INF, INF, INF, 1, 0}
        };

        // Distribui as submatrizes para os outros processos
        for (int dest = 1; dest < size; dest++) {
            MPI_Send(&matrix[0][0], n * n, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
    } else {
        // Recebe a submatriz do processo 0
        MPI_Recv(&submatrix[0][0], n * n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Algoritmo de Floyd-Warshall nas submatrizes
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (submatrix[i][k] + submatrix[k][j] < submatrix[i][j]) {
                    submatrix[i][j] = submatrix[i][k] + submatrix[k][j];
                }
            }
        }
    }

    // Envio das submatrizes atualizadas para o processo 0
    if (rank != 0) {
        MPI_Send(&submatrix[0][0], n * n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        // Recebe submatrizes das outros processos e atualiza a matriz completa
        for (int source = 1; source < size; source++) {
            MPI_Recv(&submatrix[0][0], n * n, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Impressão da matriz completa
        printf("Matriz de custo mínima:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (submatrix[i][j] == INF) {
                    printf("0\t");  // Substitui "INF" por "0"
                } else {
                    printf("%d\t", submatrix[i][j]);
                }
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}
