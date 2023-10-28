#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 6
#define SUBMATRIX_SIZE 3

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0) {
            printf("Este programa deve ser executado com exatamente 4 processos.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // A matriz original
    int matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 2, 0, 5, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 2, 0, 0, 0, 5},
        {0, 0, 0, 0, 1, 0},
        {3, 9, 3, 0, 0, 0},
        {0, 0, 0, 0, 1, 0}
    };

    int submatrix[SUBMATRIX_SIZE][SUBMATRIX_SIZE];

    // Calcula o tamanho das submatrizes
    int submatrix_count = MATRIX_SIZE / SUBMATRIX_SIZE;
    int submatrix_size = SUBMATRIX_SIZE * SUBMATRIX_SIZE;

    // Divide as submatrizes entre os processos
    int row_start = (rank / 2) * SUBMATRIX_SIZE;
    int col_start = (rank % 2) * SUBMATRIX_SIZE;

    for (int i = 0; i < SUBMATRIX_SIZE; i++) {
        for (int j = 0; j < SUBMATRIX_SIZE; j++) {
            submatrix[i][j] = matrix[row_start + i][col_start + j];
        }
    }

    // Comunicação entre os processos para reunir as submatrizes
    int gathered_submatrices[4][SUBMATRIX_SIZE][SUBMATRIX_SIZE];
    MPI_Allgather(submatrix, submatrix_size, MPI_INT, gathered_submatrices, submatrix_size, MPI_INT, MPI_COMM_WORLD);

    // Impressão das submatrizes por processo
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            printf("Processo %d:\n", i);
            for (int j = 0; j < SUBMATRIX_SIZE; j++) {
                for (int k = 0; k < SUBMATRIX_SIZE; k++) {
                    printf("%d ", gathered_submatrices[i][j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
