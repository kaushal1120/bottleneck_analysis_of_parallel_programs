#include <iostream>
using namespace std;
#include <omp.h>
#include <vector>

#define N 25000

void omp_overhead(int nthreads, vector<vector<float>>& a, vector<vector<float>>& b, vector<vector<float>>& c){
    for(int i=0; i<N; ++i){
        #pragma omp parallel for num_threads(nthreads)
        for(int j=0; j<N; ++j){
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

void omp_overhead_nil(int nthreads, vector<vector<float>>& a, vector<vector<float>>& b, vector<vector<float>>& c){
    omp_set_num_threads(nthreads);

    #pragma omp parallel for
    for(int i=0; i<N; ++i){
        for(int j=0; j<N; ++j){
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

int main(int argc, char *argv[])
{
    int nthreads = atoi(argv[1]);
    bool enable_overhead = atoi(argv[2]);

    vector<vector<float>> a(N,vector<float>(N,1));
    vector<vector<float>> b(N,vector<float>(N,1));
    vector<vector<float>> c(N,vector<float>(N,0));

    enable_overhead ? omp_overhead(nthreads,a,b,c) : omp_overhead_nil(nthreads,a,b,c);
    return 0;
}