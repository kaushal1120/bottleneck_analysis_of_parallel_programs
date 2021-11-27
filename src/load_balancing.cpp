#include <iostream>
using namespace std;
#include <omp.h>

#define NUM 10000000

int isprime(int x)
{
    for(int y = 2; y*y <= x; y++)
    {
        if(x % y == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char* argv[])
{
    int nthreads = atoi(argv[1]);
    bool enable_overhead = atoi(argv[2]);
    double thread_time[nthreads];

    omp_set_num_threads(nthreads);

    if(enable_overhead){
        #pragma omp parallel for
        for(int i = NUM; i >= 2; i--)
        {
            double start = omp_get_wtime();
            isprime(i);
            thread_time[omp_get_thread_num()] += omp_get_wtime()-start;
        }
    }
    else{
        #pragma omp parallel for schedule(dynamic,4)
        for(int i = NUM; i >= 2 ; i--)
        {
            double start = omp_get_wtime();
            isprime(i);
            thread_time[omp_get_thread_num()] += omp_get_wtime()-start;
        }
    }

    for(int i = 0; i < nthreads; i++){
        cout<<thread_time[i]<<endl;
    }

    return 0;
}