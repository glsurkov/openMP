#include <iostream>
#include <time.h>
#include <stdio.h>
#include <omp.h>
#include <chrono>

void printMatrix(int** matrix, int n) {
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            printf(" %d", matrix[i][j]);
        }
        printf("\n");
    }
}

void generateMatrix(int** matrix, int n, int maxInitValue) {
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            if (i == j){
                matrix[i][j] = i;
                continue;
            }

            matrix[i][j] = n + ((i + j) % maxInitValue);
        }
    }
}

void searchMaxMin(int** matrix, int n)
{
    int max = 0;
    int i = 0;
    int j = 0;

    #pragma omp parallel for private(i, j) shared(matrix) reduction(max : max)
    for (i = 0; i < n; ++i)
    {
        int minRowValue = matrix[i][0];
        #pragma omp parallel for shared(matrix)
        for (j = 1; j < n; ++j)
        {
            if (matrix[i][j] < minRowValue)    
            {       
                minRowValue = matrix[i][j];
            }
        }
        max = minRowValue;
    }

    printf("Max value = %d \n", max);
}

int main() {
    srand(time(NULL));
    int n = 100;
    int maxInitValue = 100;
    int thr = 4;
    int maxValue = 0;

    for(int k = 0; k < 4; ++k)
    {
        printf("\n\n===== Iteration %d =====\n\n", k+1);

        printf("N = %d\n", n);

        int** matrix = new int * [n];
        for (int i = 0; i < n; ++i)
            *(matrix + i) = new int [n];

        generateMatrix(matrix, n, maxInitValue);

        for(int i = 0; i < 4; ++i)
        {
            printf("Number threads = %d\n", thr);
            double timeStart = omp_get_wtime();
            searchMaxMin(matrix, n);
            double timeDif = omp_get_wtime() - timeStart;
            printf("Parallel time = %f\n", timeDif);

            thr *= 2;
        }

        // printMatrix(matrix, n);

        for (int i = 0; i < n; i++)
        {
            delete[] matrix[i];
        }
        delete[] matrix;

        n *= 5;
        maxInitValue *= 5;
    }
}