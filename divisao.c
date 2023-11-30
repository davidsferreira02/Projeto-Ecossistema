#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Imprime o nome do ficheiro da matriz para ler\n");
        return 1;
    }

    MPI_Init(&argc, &argv);

    int n, rank, size;
    int q = 0;
    int p = 0;
    int div = 0;
    int submatriz_dim; // Variável para a dimensão da submatriz
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        FILE *file = fopen(argv[1], "r");

        if (file == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            MPI_Finalize();
            return 1;
        }

        fscanf(file, "%d", &n);
        fclose(file);

        for (int i = 2; i < n; i++) {
            if (n % i == 0) {
                q = i;
                p = q * q;
                div = n / i;
                break;
            }
        }

        if (p == 0 && div == 0 && q == 0) {
            printf("Impossivel usar este algoritmo com este tamanho da matriz\n");
            MPI_Finalize();
            return 1;
        }

        

        submatriz_dim = n / q; 
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&submatriz_dim, 1, MPI_INT, 0, MPI_COMM_WORLD); 
    int matriz[n][n];

    if (rank == 0) {
        FILE *file = fopen(argv[1], "r");

        if (file == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fscanf(file, "%d", &n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fscanf(file, "%d", &matriz[i][j]);
            }
        }

        fclose(file);
    }

    MPI_Bcast(matriz, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    int submatriz[submatriz_dim][submatriz_dim];

    int submatrix_size = submatriz_dim * submatriz_dim;

   
    int row_start = (rank / 2) * submatriz_dim;
    int col_start = (rank % 2) * submatriz_dim;

    for (int i = 0; i < submatriz_dim; i++) {
        for (int j = 0; j < submatriz_dim; j++) {
            submatriz[i][j] = matriz[row_start + i][col_start + j];
        }
    }




    MPI_Barrier(MPI_COMM_WORLD); 


    for (int process_rank = 0; process_rank < size; process_rank++) {
        if (rank == process_rank) {
            printf("Submatriz do processo %d (DIMENSAO %d):\n", rank, submatriz_dim);
            for (int i = 0; i < submatriz_dim; i++) {
                for (int j = 0; j < submatriz_dim; j++) {
                    printf("%d ", submatriz[i][j]);
                }
                printf("\n");
            }
        }
        MPI_Barrier(MPI_COMM_WORLD); 
    }

    MPI_Finalize();

    return 0;
}