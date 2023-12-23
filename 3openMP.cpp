#include <iostream>
#include <omp.h>
#include <stdio.h>

float f(float x)
{
  return x*x + 3*x + 4;
}


int main()
{
  int n = 10000;
  float a = 0;
  float b = 1000;
  float h = (b - a) / n;
  float sum = 0;
  int thr = 4;

  for(int k = 0; k < 4; ++k)
  {
    printf("=====Iteration %d=====\n\n", k + 1);

    printf("N = %d\n", n);

    double timeStart = omp_get_wtime();

    for (int i = 0; i < n; ++i)
    {
        float x = i*h;
        sum += h * f(x);
    }

  printf("Integral = %f\n", sum);

  double timeDif1 = (omp_get_wtime() - timeStart);

  printf("Consist time = %f\n", timeDif1);
  printf("\n");

  for (int j = 0; j < 4; j++)
  {
  sum = 0;

  timeStart = omp_get_wtime();

  omp_set_num_threads(thr);

  printf("Threads = %d\n", thr);

  #pragma omp parallel shared(n, a, b, h)
    {
        #pragma omp for schedule(guided) reduction(+: sum)
        for (int i = 0; i < n; ++i)
        {
            float x = i*h;
            sum += h * f(x);
        }
    }
  
    printf("Integral = %f\n", sum);

    double timeDif2 = (omp_get_wtime() - timeStart);

    printf("Parallel time = %f\n", timeDif2);

    double accel = timeDif1/timeDif2;

    printf("Acceleration = %f\n", accel);
    printf("\n");

    thr *= 2;
  }

    n *= 10;
    thr = 4;
    h = (b - a) / n;
    sum = 0;
    printf("\n\n");
  }
}
