#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctime>

int main()
{
  int n = 10000;
  int thr = 4;
  int* a = new int[n];

  srand(static_cast<unsigned int>(time(0)));

  for (int j = 0; j < 4; j++)
  {
      printf("\n\n====Iteration %d====\n", j+1);
    int* a = new int[n];
    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 81515 - 1;
    }

    int minVal = 99999;

    double timeStart = omp_get_wtime();

    for (int i = 0; i < n; ++i)
        if (minVal > a[i])
		minVal = a[i];

    double timeDif1 = (omp_get_wtime() - timeStart);

    printf("\nConsistent time = %f\n", timeDif1);

    for (int k = 0; k < 4; k++)
    {
        minVal = 99999;
        omp_set_num_threads(thr);
        int chunk = n / thr;
        printf("N = %d\n", n);
        printf("Threads = %d\n", thr);
        timeStart = omp_get_wtime();

        #pragma omp parallel shared(a, n)
        {
                #pragma omp for schedule(dynamic, chunk) reduction (min:minVal)
                    for (int i = 0; i < n; ++i)
                        if (a[i] < minVal)
					                  minVal = a[i];
        }

        double timeDif2 = (omp_get_wtime() - timeStart);

        printf("Parallel time = %f\n", timeDif2);

        double accel = timeDif1/timeDif2;

        printf("Acceleration = %f\n", timeDif1/timeDif2);

        thr *= 2;
    }
    n *= 10;
    thr = 4;
    printf("Min element = %d\n", minVal);
    delete[] a;
  }
}
