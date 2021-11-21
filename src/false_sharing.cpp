#include <iostream>
using namespace std;
#include <omp.h>
#include <vector>
#include <atomic>

#define CACHE_LINE 64
#define INT_SIZE 4
#define LIMIT 100000000

void false_sharing(int values[]){
    int thread_no = omp_get_thread_num();
    printf("Thread %d : Address %p\n",thread_no,&values[thread_no]);
    for(int i = 0; i < LIMIT; i++) {
        values[thread_no]++;
    }
}

void false_sharing_nil(int values[]){
    int thread_no = omp_get_thread_num();
    printf("Thread %d : Address %p\n",thread_no,&values[thread_no*CACHE_LINE/INT_SIZE]);
    for(int i = 0; i < LIMIT; ++i) {
        values[thread_no*CACHE_LINE/INT_SIZE]++;
    }
}

int main(int argc, char* argv[]){
    int thread_count = atoi(argv[1]);

    bool enable_false_sharing = atoi(argv[2]);

    int values[thread_count*CACHE_LINE/INT_SIZE + 1] = {};

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