#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N (30)

int main(int argc, char *argv[])
{
    int nthreads, tid, idx,idj;
    float a[N][N], b[N][N], c[N][N];
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);

    omp_set_num_threads(1);

    // init
    #pragma omp parallel for
    for(idx=0; idx<N; ++idx)
    {
        #pragma omp parallel for
        for(idj=0; idj<N; ++idj)
        {
             a[idx][idj] = b[idx][idj] = 1.0;
        }
    }

    // vec add
    #pragma omp parallel for
    for(idx=0; idx<N; ++idx)
    {
        #pragma omp parallel for
        for(idj=0; idj<N; ++idj)
        {
            c[idx][idj] = a[idx][idj] + b[idx][idj];
        }
    }

    return 0;
}