#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <chrono>

int* generateArray(int n)
{
    int* array = new int[n];

    for (int i = 0; i < n; i++)
    {
        array[i] = rand() % 5 + (rand() % 3);
    }
    return array;
}

void printArray(int* array, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ",array[i]);
    }
    printf("\n");
}

void staticApproach(int* array, int n)
{
    printf("\n===== Static =====\n");

    int maxValue = 0;

    #pragma omp parallel shared(array, n)
    {
        #pragma omp for schedule(static) reduction(max : maxValue)
            for (int i = 0; i < n; ++i)
            {
                if (i % 2 == 0)
                {
                    int temp = rand() * 1100 % 7;
                }

                if (array[i] > maxValue)
                {
                    maxValue = array[i];
                }
            }
    }
    printf("Max = %d \n", maxValue);
}

void dynamicApproach(int* array, int n)
{
    printf("\n===== Dynamic =====\n");

    int maxValue = 0;

    #pragma omp parallel shared(array, n)
    {
        #pragma omp for schedule(dynamic) reduction(max : maxValue)
            for (int i = 0; i < n; ++i)
            {
                if (i % 3 == 1)
                {
                    int temp = rand() % 10;
                }
                if (array[i] > maxValue)
                {
                    maxValue = array[i];
                }
            }
    }
    printf("Max = %d \n", maxValue);
}

void guidedApproach(int* array, int n)
{
    printf("\n===== Guided =====\n");

    int maxValue = 0;

    #pragma omp parallel shared(array, n)
    {
        #pragma omp for schedule(guided) reduction(max : maxValue)
        for (int i = 0; i < n; ++i)
        {
            if (i % 3 == 1)
                {
                    int temp = rand() % 10;
                }
            if (array[i] > maxValue)
                {
                    maxValue = array[i];
                }
        }
    }
    printf("Max = %d \n", maxValue);
}

int main()
{
    srand(time(NULL));
    int n = 10000;

    for(int i = 0; i < 4; ++i)
    {
        printf("\n\n===== Iteration %d =====\n\n", i + 1);

    int* array = generateArray(n);

    // printArray(array, n);

    double timeStart = omp_get_wtime();
    staticApproach(array, n);
    double timeDif = omp_get_wtime() - timeStart;
    printf("Static time = %f\n", timeDif);

    timeStart = omp_get_wtime();
    dynamicApproach(array, n);
    timeDif = omp_get_wtime() - timeStart;
    printf("Dynamic time = %f\n", timeDif);
    
    timeStart = omp_get_wtime();
    guidedApproach(array, n);
    timeDif = omp_get_wtime() - timeStart;
    printf("Dynamic time = %f\n", timeDif);

    delete[] array;

    n *= 10;
    }
}