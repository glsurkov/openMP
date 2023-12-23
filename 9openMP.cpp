#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>

void pingThreads(int level)
{
    #pragma omp single
    {   
        printf("Nested level = %d ; threads = %d\n",level, omp_get_num_threads());
    }
}

void nestedFragments(int thr)
{
    omp_set_num_threads(thr);
    omp_set_dynamic(0);

    double time_start = omp_get_wtime();
    #pragma omp parallel
    {
        pingThreads(1);
        #pragma omp parallel
        {
            pingThreads(2);
            #pragma omp parallel
            {
                pingThreads(3);
            }
        }
    }
    double timeDif = (omp_get_wtime() - time_start);
    printf("Time: %f threads = %d\n", timeDif, thr);
}

int main()
{
    int thr = 2;

    for (int i = 0; i < 4; ++i)
    {
        nestedFragments(thr);
        thr += 2;
    }

    printf("Increase number of nested levels \n");
    // omp_set_max_active_levels(7);
    omp_set_nested(true);

    thr = 2;

    for (int i = 0; i < 4; ++i)
    {
        nestedFragments(thr);
        thr += 2;
    }

}