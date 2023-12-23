#include <omp.h>
#include <stdio.h>
#include <time.h>

int main()
{
  int iterationCount = 1000000;
  int sum = 0;
  int thr = 2;

  printf("\n");
  printf("==== Default ====");
  printf("\n");

  int chunkSize = 10000;

  double timeStart = omp_get_wtime();

  for (int i = 0; i < iterationCount; ++i)
  {
    sum += 1;
  }

  double timeSpent = omp_get_wtime() - timeStart;
  printf("Sum = %d; Reduction spent time = %f\n", sum, timeSpent);

  for(int k = 0; k < 4; ++k)
  {
    printf("\n====== Iteration %d ======\n", k + 1);

    omp_set_num_threads(thr);
    printf("Number of threads = %d\n", thr);

    printf("\n");
    printf("==== Reduction directive ====");
    printf("\n");

    timeStart = omp_get_wtime();

    sum = 0;

    #pragma omp parallel for schedule(dynamic, chunkSize) reduction(+ : sum)
      for (int i = 0; i < iterationCount; ++i)
      {
        sum += 1;
      }

    timeSpent = omp_get_wtime() - timeStart;
    printf("Sum = %d; Reduction spent time = %f\n", sum, timeSpent);
    sum = 0;

    printf("\n");
    printf("==== Atomic directive ====");
    printf("\n");
    timeStart = omp_get_wtime();

    #pragma omp parallel for shared(sum) schedule(dynamic, chunkSize)
      for (int i = 0; i < iterationCount; ++i)
      {
        #pragma omp atomic
          sum += 1;
      }

    timeSpent = omp_get_wtime() - timeStart;
    printf("Sum = %d; Atomic spent time = %f\n", sum, timeSpent);
    sum = 0;

    printf("\n");
    printf("==== Critical directive ====");
    printf("\n");
    timeStart = omp_get_wtime();

    #pragma omp parallel for shared(sum) schedule(dynamic, chunkSize)
      for (int i = 0; i < iterationCount; ++i)
      {
        #pragma omp critical
          sum += 1;
      }

    timeSpent = omp_get_wtime() - timeStart;
    printf("Sum = %d; Critical spent time = %f\n", sum, timeSpent);
    sum = 0;

    printf("\n");
    printf("==== Lock ====");
    printf("\n");

    omp_lock_t lock;
    omp_init_lock(&lock);
    timeStart = omp_get_wtime();

    #pragma omp parallel for shared(sum) schedule(dynamic, chunkSize)
      for (int i = 0; i < iterationCount; ++i)
      {
        omp_set_lock(&lock);
        sum += 1;
        omp_unset_lock(&lock);
      }

    timeSpent = omp_get_wtime() - timeStart;
    printf("Sum = %d; Lock spent time = %f\n", sum, timeSpent);
    sum = 0;

    thr += 2;
  }
}
