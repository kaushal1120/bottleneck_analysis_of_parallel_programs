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
    int sum = 0;
    int nthreads = atoi(argv[1]);
    bool enable_overhead = atoi(argv[2]);

    omp_set_num_threads(nthreads);

    if(enable_overhead){
        #pragma omp parallel for schedule(dynamic,4)
        for(int i = NUM; i >= 2 ; i--)
        {
            #pragma omp critical
            sum += isprime(i);
        }
    }
    else{
        #pragma omp parallel for schedule(dynamic,4)
        for(int i = NUM; i >= 2 ; i--)
        {
            int local_sum = isprime(i);
            #pragma omp critical
            sum += local_sum;
        }
    }
    printf("Sum: %d\n",sum);

    return 0;
}