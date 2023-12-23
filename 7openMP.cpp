#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>

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


void scalarMult(int thr, int n)
{
    omp_set_num_threads(thr);

    int** vectors = new int*[n];
    int* vMult = new int[n];

    for( int i = 0; i < n; ++i)
    {
        vMult[i] = 1;
    }
    int* output = 0;
    int currentGeneredLine = 0;

    omp_lock_t lock;
    omp_init_lock(&lock);

    double time_start = omp_get_wtime();
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int i = 0; i < n; ++i)
            {
                omp_set_lock(&lock);
                int* a = new int[n];
                for (int j = 0; j < n; ++j)
                {
                    a[j] = i * j;
                }
                vectors[i] = a;
                currentGeneredLine = i;
                omp_unset_lock(&lock);
            }
        }  
        #pragma omp section
        {
            for (int i = 1; i < n; ++i)
            {
                if(currentGeneredLine < i)
                {
                    omp_set_lock(&lock);
                    omp_unset_lock(&lock);
                }
                for (int j = 0; j < n; ++j)
                {
                    vMult[j] *= vectors[i][j];
                }
            }
            for (int i = 0; i < n; ++i)
            {
                output += vMult[i];
            }
            // printf("scalar multiplication %d\n", output);
        }
    }

    double timeDif = (omp_get_wtime() - time_start);
    // printMatrix(vectors, n);
    printf("time: %f threads = %d\n", timeDif, thr);
}

int main()
{
    int thr = 4;
    int n = 1000;

for(int i = 0; i < 4; ++i)
    {
        for (int i = 0; i < 4; i++)
        {
            scalarMult(thr, n);
            thr *= 2;
        }
        n *= 2;
        thr = 4;
    }
}