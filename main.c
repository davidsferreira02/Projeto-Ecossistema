#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define INF 99999

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int n, rank, size;
    int p,q,div;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


  if(rank==0){
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
                div = n / q;
                break;
            }
        }

        if (p == 0 && div == 0 && q == 0) {
            printf("Impossivel usar este algoritmo com este tamanho da matriz\n");
            MPI_Finalize();
            return 1;
        }

  }
    
    
 MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int submatrix_size =div*div;
    int submatrix[n][n];
    int matrix[n][n];
    
    
    
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                submatrix[i][j] = 0;
            } else {
                submatrix[i][j] = INF;
            }
        }
    }
    
    
    if (rank == 0) {
          FILE *file = fopen(argv[1], "r");

        if (file == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fscanf(file, "%d", &n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fscanf(file, "%d", &matrix[i][j]);
            }
        }

        fclose(file);
        
       
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(i!=j && matrix[i][j]==0){
                    matrix[i][j]=INF;
                }
            }
        }
    }



        MPI_Barrier(MPI_COMM_WORLD); 
        for (int dest = 1; dest < size; dest++) {
            MPI_Send(&matrix, n * n, MPI_INT, dest, 0, MPI_COMM_WORLD);
           
        }
    MPI_Bcast(matrix, n * n, MPI_INT, 0, MPI_COMM_WORLD);

   






    
    if(rank!=0) {
       
        MPI_Recv(&submatrix, n * n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        
    }


    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (submatrix[i][k] + submatrix[k][j] < submatrix[i][j]) {
                    submatrix[i][j] = submatrix[i][k] + submatrix[k][j];
                }
            }
        }
    }


  
    if (rank != 0) {
        MPI_Send(&submatrix, n * n, MPI_INT, 0, 0, MPI_COMM_WORLD);
        
       
        
    } else {
       
        for (int source = 1; source < size; source++) {
            MPI_Recv(&submatrix, n * n, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
             


        }


        printf("Matriz de custo mínima:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (submatrix[i][j] == INF) {
                    printf("0\t"); 
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