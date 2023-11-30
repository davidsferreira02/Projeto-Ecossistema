#include <stdio.h>
#include <omp.h>
#include <string.h>

#define MAX 100

typedef struct
{
    char tipo[10];
    int x, y;
} Objeto;

typedef enum
{
    EMPTY,
    ROCK,
    RABBIT,
    FOX
} TipoCelula;

typedef struct
{
    TipoCelula tipo;
    int procreation_age;
    int isMoved;
} Celula;


TipoCelula stringToTipoCelula(const char *str)
{
    if (strcmp(str, "ROCK") == 0)
    {
        return ROCK;
    }
    else if (strcmp(str, "RABBIT") == 0)
    {
        return RABBIT;
    }
    else if (strcmp(str, "FOX") == 0)
    {
        return FOX;
    }
    else
    {
        return EMPTY;
    }
}


void Resultado(Celula ecossistema[MAX][MAX], int rows, int columns, int gen)
{
    printf("Gen %d \n", gen);
    printf("-----\n");

    for (int i = 0; i < rows; i++)
    {
        printf("|");
        for (int j = 0; j < columns; j++)
        {
            switch (ecossistema[i][j].tipo)
            {
            case EMPTY:
                printf(" ");
                break;
            case ROCK:
                printf("*");
                break;
            case RABBIT:
                printf("R");
                break;
            case FOX:
                printf("F");
                break;
            }
        }
        printf("|");
        printf("\n");
    }
}

void moveRabbits(Celula ecossistema[MAX][MAX], int rows, int columns, int genProc, int generation)
{
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            int emptycell = 0;

            if (ecossistema[i][j].tipo == RABBIT && ecossistema[i][j].isMoved == 0)
            {
                if (ecossistema[i - 1][j].tipo == EMPTY && i - 1 >= 0)
                {
                    emptycell++;
                }

                if (ecossistema[i][j + 1].tipo == EMPTY && j + 1 <= columns)
                {
                    emptycell++;
                }

                if (ecossistema[i + 1][j].tipo == EMPTY && i + 1 <= rows)
                {
                    emptycell++;
                }

                if (ecossistema[i][j - 1].tipo == EMPTY && j - 1 >= 0)
                {
                    emptycell++;
                }

                if (emptycell == 0)
                {
                    // Não há células vazias adjacentes
                    ecossistema[i][j].isMoved = 1; // Marcando como movido nesta geração
                }
                else
                {
                    int nextCell = (generation + i + j) % emptycell;

                    if (nextCell == 0)
                    {
                        if (i > 0 && ecossistema[i - 1][j].tipo == EMPTY)
                        {
                            // Movendo para cima se a célula acima estiver vazia
                            ecossistema[i - 1][j].tipo = RABBIT;

                            if (ecossistema[i][j].procreation_age != genProc)
                            {
                                ecossistema[i - 1][j].procreation_age = ecossistema[i][j].procreation_age + 1;
                                ecossistema[i][j].tipo = EMPTY;
                                ecossistema[i][j].procreation_age = 0;
                            }
                            else
                            {
                                ecossistema[i][j].tipo = RABBIT;
                                ecossistema[i][j].isMoved = 1;
                                ecossistema[i][j].procreation_age = 0;
                                ecossistema[i - 1][j].procreation_age = 0;
                            }
                            ecossistema[i - 1][j].isMoved = 1;
                        }
                        else if (j < columns - 1 && ecossistema[i][j + 1].tipo == EMPTY)
                        {
                            // Movendo para a direita se a célula à direita estiver vazia
                            ecossistema[i][j + 1].tipo = RABBIT;

                            if (ecossistema[i][j].procreation_age != genProc)
                            {
                                ecossistema[i][j + 1].procreation_age = ecossistema[i][j].procreation_age + 1;
                                ecossistema[i][j].tipo = EMPTY;
                                ecossistema[i][j].procreation_age = 0;
                            }
                            else
                            {
                                ecossistema[i][j].tipo = RABBIT;
                                ecossistema[i][j].isMoved = 1;
                                ecossistema[i][j].procreation_age = 0;
                                ecossistema[i][j + 1].procreation_age = 0;
                            }
                            ecossistema[i][j + 1].isMoved = 1;
                        }
                        else if (i < rows - 1 && ecossistema[i + 1][j].tipo == EMPTY)
                        {
                            // Movendo para baixo se a célula abaixo estiver vazia
                            ecossistema[i + 1][j].tipo = RABBIT;

                            if (ecossistema[i][j].procreation_age != genProc)
                            {
                                ecossistema[i + 1][j].procreation_age = ecossistema[i][j].procreation_age + 1;
                                ecossistema[i][j].tipo = EMPTY;
                                ecossistema[i][j].procreation_age = 0;
                            }
                            else
                            {
                                ecossistema[i][j].tipo = RABBIT;
                                ecossistema[i][j].isMoved = 1;
                                ecossistema[i][j].procreation_age = 0;
                                ecossistema[i + 1][j].procreation_age = 0;
                            }
                            ecossistema[i + 1][j].isMoved = 1;
                        }
                        else if (j > 0 && ecossistema[i][j - 1].tipo == EMPTY)
                        {
                            // Movendo para a esquerda se a célula à esquerda estiver vazia
                            ecossistema[i][j - 1].tipo = RABBIT;

                            if (ecossistema[i][j].procreation_age != genProc)
                            {
                                ecossistema[i][j - 1].procreation_age = ecossistema[i][j].procreation_age + 1;
                                ecossistema[i][j].tipo = EMPTY;
                                ecossistema[i][j].procreation_age = 0;
                            }
                            else
                            {
                                ecossistema[i][j].tipo = RABBIT;
                                ecossistema[i][j].isMoved = 1;
                                ecossistema[i][j].procreation_age = 0;
                                ecossistema[i][j - 1].procreation_age = 0;
                            }
                            ecossistema[i + 1][j].isMoved = 1;
                        }
                        else
                        {
                            // Não há células vazias adjacentes
                            ecossistema[i][j].isMoved = 1; // Marcando como movido nesta geração
                        }
                    }

                    if (nextCell == 1)
                    {
                        int contador = 0;
                        if (ecossistema[i - 1][j].tipo == EMPTY)
                        {
                            contador++;
                        }
                        if (ecossistema[i][j + 1].tipo == EMPTY)
                        {
                            contador++;
                            if (contador == 2)
                            {
                                if (ecossistema[i][j].procreation_age != genProc)
                                {
                                    ecossistema[i][j + 1].tipo = RABBIT;
                                    ecossistema[i][j + 1].isMoved = 1;
                                    ecossistema[i][j + 1].procreation_age = ecossistema[i][j].procreation_age + 1;
                                    ecossistema[i][j].isMoved = 1;
                                    ecossistema[i][j].tipo = EMPTY;
                                    ecossistema[i][j].procreation_age = 0;
                                }
                                else
                                {
                                    ecossistema[i][j + 1].tipo = RABBIT;
                                    ecossistema[i][j + 1].isMoved = 1;
                                    ecossistema[i][j + 1].procreation_age = 0;
                                    ecossistema[i][j].procreation_age = 0;
                                    ecossistema[i][j].isMoved = 1;
                                }
                            }
                        }
                        if (ecossistema[i + 1][j].tipo == EMPTY && contador != 2)
                        {
                            contador++;
                            if (contador == 2)
                                if (ecossistema[i][j].procreation_age != genProc)
                                {
                                    ecossistema[i + 1][j].tipo = RABBIT;
                                    ecossistema[i + 1][j].isMoved = 1;
                                    ecossistema[i + 1][j].procreation_age = ecossistema[i][j].procreation_age + 1;
                                    ecossistema[i][j].isMoved = 1;
                                    ecossistema[i][j].tipo = EMPTY;
                                    ecossistema[i][j].procreation_age = 0;
                                }else
                                {
                                    ecossistema[i + 1][j].tipo = RABBIT;
                                    ecossistema[i + 1][j].isMoved = 1;
                                    ecossistema[i + 1][j].procreation_age = 0;
                                    ecossistema[i][j].procreation_age = 0;
                                    ecossistema[i][j].isMoved = 1;
                                }
                        }
                        if (ecossistema[i][j - 1].tipo == EMPTY && contador < 2)
                        {
                            contador++;
                            if (contador == 2)
                            {

                                if (ecossistema[i][j].procreation_age != genProc)
                                {
                                    ecossistema[i][j - 1].tipo = RABBIT;
                                    ecossistema[i][j - 1].isMoved = 1;
                                    ecossistema[i][j - 1].procreation_age = ecossistema[i][j].procreation_age + 1;
                                    ecossistema[i][j].isMoved = 1;
                                    ecossistema[i][j].tipo = EMPTY;
                                    ecossistema[i][j].procreation_age = 0;
                                }
                                else
                                {
                                    ecossistema[i][j - 1].tipo = RABBIT;
                                    ecossistema[i][j - 1].isMoved = 1;
                                    ecossistema[i][j - 1].procreation_age = 0;
                                    ecossistema[i][j].procreation_age = 0;
                                    ecossistema[i][j].isMoved = 1;
                                }
                            }
                        }
                    }

                    if (nextCell == 2)
                    {
                        int contador = 0;

                        if (i - 1 > 0 && ecossistema[i - 1][j].tipo == EMPTY && contador < 3)
                        {

                            printf(" 1 Entrou na posiçao (%d,%d) e contador = %d\n", i, j, contador);
                            contador++;
                            if (contador == 3)
                            {
                                if (ecossistema[i][j].procreation_age != genProc)
                                {
                                    ecossistema[i - 1][j].tipo = RABBIT;
                                    ecossistema[i - 1][j].isMoved = 1;
                                    ecossistema[i - 1][j].procreation_age = ecossistema[i][j].procreation_age + 1;
                                    ecossistema[i][j].isMoved = 1;
                                    ecossistema[i][j].tipo = EMPTY;
                                    ecossistema[i][j].procreation_age = 0;
                                }
                                else
                                {
                                    ecossistema[i - 1][j].tipo = RABBIT;
                                    ecossistema[i - 1][j].isMoved = 1;
                                    ecossistema[i - 1][j].procreation_age = 0;
                                    ecossistema[i][j].procreation_age = 0;
                                    ecossistema[i][j].isMoved = 1;
                                }
                            }
                        }
                        if (j + 1 < columns && ecossistema[i][j + 1].tipo == EMPTY && contador < 3)
                        {
                           printf("2 Entrou na posiçao (%d,%d) e contador = %d\n", i, j, contador);
                            contador++;
                            if (contador == 3)
                            {
                                if (ecossistema[i][j].procreation_age != genProc)
                                {
                                    ecossistema[i][j + 1].tipo = RABBIT;
                                    ecossistema[i][j + 1].isMoved = 1;
                                    ecossistema[i][j + 1].procreation_age = ecossistema[i][j].procreation_age + 1;
                                    ecossistema[i][j].isMoved = 1;
                                    ecossistema[i][j].tipo = EMPTY;
                                    ecossistema[i][j].procreation_age = 0;
                                }
                                else
                                {
                                    ecossistema[i][j + 1].tipo = RABBIT;
                                    ecossistema[i][j + 1].isMoved = 1;
                                    ecossistema[i][j + 1].procreation_age = 0;
                                    ecossistema[i][j].procreation_age = 0;
                                    ecossistema[i][j].isMoved = 1;
                                }
                            }
                        }
                        if (i + 1 < rows && ecossistema[i + 1][j].tipo == EMPTY && contador < 3)
                        {
                           printf("3 Entrou na posiçao (%d,%d) e contador = %d ,proc_age= %d\n", i, j, contador,ecossistema[i][j].procreation_age);
                            contador++;
                            if (contador == 3)
                            {
                                if (ecossistema[i][j].procreation_age != genProc)
                                {
                                    ecossistema[i + 1][j].tipo = RABBIT;
                                    ecossistema[i + 1][j].isMoved = 1;
                                    ecossistema[i + 1][j].procreation_age = ecossistema[i][j].procreation_age + 1;
                                    ecossistema[i][j].isMoved = 1;
                                    ecossistema[i][j].tipo = EMPTY;
                                    ecossistema[i][j].procreation_age = 0;
                                }
                               
                                else
                                {
                                    ecossistema[i + 1][j].tipo = RABBIT;
                                    ecossistema[i + 1][j].isMoved = 1;
                                    ecossistema[i + 1][j].procreation_age = 0;
                                    ecossistema[i][j].procreation_age = 0;
                                    ecossistema[i][j].isMoved = 1;
                                }
                             
                            }
                        }
                        if (j - 1 > 0 && ecossistema[i][j - 1].tipo == EMPTY && contador < 3)
                        {
                            printf("4 Entrou na posiçao (%d,%d) e contador = %d\n", i, j, contador);
                            contador++;
                            if (contador == 3)
                            {
                                if (ecossistema[i][j].procreation_age != genProc)
                                {
                                    ecossistema[i][j - 1].tipo = RABBIT;
                                    ecossistema[i][j - 1].isMoved = 1;
                                    ecossistema[i][j - 1].procreation_age = ecossistema[i][j].procreation_age + 1;
                                    ecossistema[i][j].isMoved = 1;
                                    ecossistema[i][j].tipo = EMPTY;
                                    ecossistema[i][j].procreation_age = 0;
                                }
                                else
                                {
                                    ecossistema[i][j - 1].tipo = RABBIT;
                                    ecossistema[i][j - 1].isMoved = 1;
                                    ecossistema[i][j - 1].procreation_age = 0;
                                    ecossistema[i][j].procreation_age = 0;
                                    ecossistema[i][j].isMoved = 1;
                                }
                            }
                        }
                        // Se contador for menor que 2, significa que não foi possível mover para a esquerda.
                        if (contador != 3)
                        {
                            ecossistema[i][j].isMoved = 1; // Marcando como movido nesta geração
                        }
                    }

                    if (nextCell == 3)
                    {
                        int contador = 0;
                        if (ecossistema[i - 1][j].tipo == EMPTY && contador < 4)
                        {
                            contador++;
                        }
                        if (ecossistema[i][j + 1].tipo == EMPTY && contador < 4)
                        {
                            contador++;
                        }
                        if (ecossistema[i + 1][j].tipo == EMPTY && contador < 4)
                        {
                            contador++;
                        }
                        if (ecossistema[i][j - 1].tipo == EMPTY && contador < 4)
                        {
                            contador++;
                            if (contador == 4)
                            {
                                if (ecossistema[i][j].procreation_age != genProc)
                                {
                                    ecossistema[i][j - 1].tipo = RABBIT;
                                    ecossistema[i][j - 1].isMoved = 1;
                                    ecossistema[i][j - 1].procreation_age = ecossistema[i][j].procreation_age + 1;
                                    ecossistema[i][j].isMoved = 1;
                                    ecossistema[i][j].tipo = EMPTY;
                                    ecossistema[i][j].procreation_age = 0;
                                }
                                else
                                {
                                    ecossistema[i][j - 1].tipo = RABBIT;
                                    ecossistema[i][j - 1].isMoved = 1;
                                    ecossistema[i][j - 1].procreation_age = 0;
                                    ecossistema[i][j].procreation_age = 0;
                                    ecossistema[i][j].isMoved = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void isMovedZero(Celula ecossistema[MAX][MAX], int rows, int columns) {
    #pragma omp parallel for collapse(2) shared(ecossistema, rows, columns)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            #pragma omp atomic write //garante que não existe interferencias de outras threads
            ecossistema[i][j].isMoved = 0;
        }
    }
}


       
        int main()
        {

            int GEN_PROC_RABBITS = 0;     // número de gerações até um coelho poder procriar
            int GEN_PROC_FOXES = 0;       // número de gerações até uma raposa poder procriar
            int GEN_FOOD_FOXES = 0;       // número de gerações para uma raposa morrer de fome
            int N_GEN = 0;                // número de gerações para a simulação
            int R = 0;                    // número de linhas da matriz que representa o ecossistema
            int C = 0;                    // número de colunas da matriz que representa o ecossistema
            int N = 0;                    // número de objectos no ecossistema inicial
            Objeto objeto[MAX];           // Para a leitura dos objetos rock,rabbit,fox
            Celula ecossistema[MAX][MAX]; // Matriz do ecossistema

            FILE *file = fopen("input.txt", "r");
            if (file == NULL)
            {
                perror("Erro ao abrir o arquivo");
                return 1;
            }
            fscanf(file, "%d %d %d %d %d %d %d", &GEN_PROC_RABBITS, &GEN_PROC_FOXES, &GEN_FOOD_FOXES, &N_GEN, &R, &C, &N);

            for (int i = 0; i < R; i++)
            {
                for (int j = 0; j < C; j++)
                {
                    ecossistema[i][j].tipo = EMPTY;
                }
            }

            #pragma omp parallel for
            for (int i = 0; i < N; i++)
            {
                fscanf(file, "%9s %d %d", objeto[i].tipo, &objeto[i].x, &objeto[i].y);
                ecossistema[objeto[i].x][objeto[i].y].tipo = stringToTipoCelula(objeto[i].tipo);
            }
            for (int i; i < R; i++)
            {
                for (int j = 0; j < C; j++)
                {
                    ecossistema[i][j].isMoved = 0;
                    ecossistema[i][j].procreation_age = 0;
                }
            }

            printf("Gen_Proc_Rabits = %d\n", GEN_PROC_RABBITS);

            printf("Gen_Proc_Foxes = %d\n", GEN_PROC_FOXES);

            printf("Gen_Food_Foxes = %d\n", GEN_FOOD_FOXES);
            printf("N_GEN = %d\n", N_GEN);

            printf("R = %d\n", R);

            printf("C = %d\n", C);

            printf("N = %d\n", N);

            for (int i = 0; i < N; i++)
            {
                printf("%s %d %d\n", objeto[i].tipo, objeto[i].x, objeto[i].y);
            }

            Resultado(ecossistema, R, C, 0);

            moveRabbits(ecossistema, R, C, GEN_PROC_RABBITS, 0);
            Resultado(ecossistema, R, C, 1);
            isMovedZero(ecossistema,R,C);
            moveRabbits(ecossistema,R,C,GEN_PROC_RABBITS,1);
            Resultado(ecossistema,R,C,2);
            isMovedZero(ecossistema,R,C);
             moveRabbits(ecossistema,R,C,GEN_PROC_RABBITS,2);
            Resultado(ecossistema,R,C,3);

            return 0;
        }
