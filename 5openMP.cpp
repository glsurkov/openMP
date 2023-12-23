#include <omp.h>
#include <stdio.h>
#include <fcntl.h>

void generateTriangleMatrix(int** matrix, int n, int maxInitValue) {
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            if (i >= j){
                matrix[i][j] = n - j;
                continue;
            }

            matrix[i][j] = 0;
        }
    }
}

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

void triangleMatrix(int** matrix,int maxValue,int thr,int maxInitValue,int n) {

int chunkSize = n / thr;
#pragma omp parallel shared(matrix, maxValue)
  {
  #pragma omp for schedule(dynamic, chunkSize)
    for (int i = 0; i < n; ++i)
    {
      int minRowValue = n + maxInitValue;

      for (int j = 0; j < i + 1; ++j)
      {
        if (matrix[i][j] < minRowValue)
        {
          minRowValue = matrix[i][j];
        }
      }

    //   printf("Min value for %d row = %d\n", i, minRowValue);

      if (minRowValue > maxValue)
      {
  #pragma omp critical
        if (minRowValue > maxValue)
        {
          maxValue = minRowValue;
        }
      }
    }
  }
}

int main()
{
    int thr = 4;
    int n = 100;
    int maxInitValue = 100;

    for(int i = 0; i < 4; ++i)
    {

        printf("\nN = %d\n", n);
        int** matrix2 = new int * [n];
        for (int i = 0; i < n; ++i)
            *(matrix2 + i) = new int [n];

        generateTriangleMatrix(matrix2, n, maxInitValue);

        // printMatrix(matrix2, n);

        for(int i = 0; i < 4; ++i)
        {
            int maxValue = 0;

            printf("Nmber of threads = %d\n", thr);

            double timeStart = omp_get_wtime();
    
            triangleMatrix(matrix2, maxValue, thr, maxInitValue, n);
    
            printf("End time = %f\n", omp_get_wtime() - timeStart);

            thr *= 2;
        }

        thr = 4;
        n *= 5;
        maxInitValue *= 5;
    }
}