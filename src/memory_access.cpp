#include <iostream>
using namespace std;
#include <omp.h>
#include <vector>

#define N 25000

int memory_access_bad(int nthreads, vector<vector<int>>& a){
    long sum = 0;
    #pragma omp parallel for num_threads(nthreads) reduction(+:sum)
    for(int i=0; i<N; ++i){
        for(int j=0; j<N; ++j){
            sum += a[j][i];
        }
    }
    return sum;
}

int memory_access_good(int nthreads, vector<vector<int>>& a){
    long sum = 0;
    #pragma omp parallel for num_threads(nthreads) reduction(+:sum)
    for(int i=0; i<N; ++i){
        for(int j=0; j<N; ++j){
            sum += a[i][j];
        }
    }
    return sum;
}

int main(int argc, char *argv[])
{
    int nthreads = atoi(argv[1]);
    bool enable_overhead = atoi(argv[2]);

    vector<vector<int>> a(N,vector<int>(N,1));

    double start = omp_get_wtime();
    long sum = enable_overhead ? memory_access_bad(nthreads,a) : memory_access_good(nthreads,a);
    double end = omp_get_wtime();
    printf("Sum: %ld, Time taken: %f\n", sum, end - start);
    return 0;
}