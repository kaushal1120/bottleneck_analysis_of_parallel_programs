#include <iostream>
using namespace std;
#include <omp.h>
#include <vector>

#define CACHE_LINE 64
#define INT_SIZE 4
#define LIMIT 100000000

void false_sharing(vector<vector<int>>& values){
    int thread_no = omp_get_thread_num();
    printf("Thread %d : Address %p\n",thread_no,&values[0][thread_no]);
    for(int i = 0; i < LIMIT; i++) {
        values[0][thread_no]++;
    }
}

void false_sharing_nil(vector<vector<int>>& values){
    int thread_no = omp_get_thread_num();
    printf("Thread %d : Address %p\n",thread_no,&values[thread_no][0]);
    for(int i = 0; i < LIMIT; ++i) {
        values[thread_no][0]++;
    }
}

int main(int argc, char* argv[]){
    int thread_count = atoi(argv[1]);

    bool enable_false_sharing = atoi(argv[2]);

    vector<vector<int>> values(thread_count,vector<int>(CACHE_LINE/INT_SIZE,0));

    if(enable_false_sharing){
        #pragma omp parallel num_threads(thread_count)
        {
            false_sharing(values);
        }
    }
    else{
        #pragma omp parallel num_threads(thread_count)
        {
            false_sharing_nil(values);
        }
    }
}