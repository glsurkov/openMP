#include <omp.h>
#include <stdio.h>
#include <fcntl.h>

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


int defaultMatrix(int** matrix, int maxInitValue, int n, int maxValue) {
    for (int i = 0; i < n; ++i)
    {
      int minRowValue = n + maxInitValue;

      for (int j = 0; j < n; ++j)
      {
        if (matrix[i][j] < minRowValue)
        {
          minRowValue = matrix[i][j];
        }
      }
      
      if (minRowValue > maxValue)
        {
          maxValue = minRowValue;
        }
    }

    return maxValue;
}


int parallelMatrix (int** matrix, int maxInitValue, int chunkSize, int n, int maxValue) {
    #pragma omp parallel shared(matrix, maxValue)
  {
  #pragma omp for schedule(dynamic, chunkSize)
    for (int i = 0; i < n; ++i)
    {
      int minRowValue = n + maxInitValue;

      for (int j = 0; j < n; ++j)
      {
        if (matrix[i][j] < minRowValue)
        {
          minRowValue = matrix[i][j];
        }
      }

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

  return maxValue;
}

int main()
{
  int n = 100;
  int maxInitValue = 100;
  int thr = 4;
  int maxValue = 0;


  for (int k = 0; k < 4; ++k)
  {
    printf("\n\n=====Iteration %d=====\n\n", k + 1);

    printf("N = %d\n", n);

    int** matrix1 = new int * [n];
    for (int i = 0; i < n; ++i)
      *(matrix1 + i) = new int [n];

    generateMatrix(matrix1, n, maxInitValue);
    // printMatrix(matrix1, n);

    double timeStart = omp_get_wtime();
    int value1 = defaultMatrix(matrix1, maxInitValue, n, maxValue);
    double timeDif1 = omp_get_wtime() - timeStart;

    printf("Maxmin value = %d\n", value1);

    printf("Consistent time = %f\n", timeDif1);

    for (int i = 0; i < 4; ++i)
    {
      printf("Number of threads = %d\n", thr);
      int chunkSize = n / thr;

      timeStart = omp_get_wtime();
      int value2 = parallelMatrix(matrix1, maxInitValue, chunkSize, n, maxValue);
      double timeDif2 = omp_get_wtime() - timeStart;

      printf("Parallel time = %f\n", timeDif2);
      printf("Acceleration = %f\n", timeDif1/timeDif2);

      thr *= 2;
    }

    n *= 5;
    maxInitValue *= 10;
    thr = 4;
  }    
}
