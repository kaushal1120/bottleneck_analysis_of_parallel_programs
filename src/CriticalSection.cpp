#include <iostream>
#include <omp.h>
#include <vector>
#include <fstream>
#include <cmath>
#include<stdio.h>
#include<stdlib.h>



using namespace std;

#define N 1000000
#define BIN_COUNT 20
#define MAX_VALUE 1000


int arr[N];
int bin[BIN_COUNT];
int global_max=-1;


void badloop(int thread_count) {


    cout << "This is Bad version "<< "\n";
    int binRange = (MAX_VALUE/BIN_COUNT);

	omp_set_num_threads(thread_count);
    #pragma omp parallel for
    for(int i = 0 ; i <N ; i++){
        #pragma omp critical
        {
        int selected_bin = floor(arr[i]/binRange);
        bin[selected_bin]++;
        }
    }

}

void goodloop(int thread_count) {

    cout << "This is good version "<< "\n";

	omp_set_num_threads(thread_count);
    int binRange = (MAX_VALUE/BIN_COUNT);
    int bin_count = 20;
    #pragma omp parallel for reduction(+:bin[:BIN_COUNT])
    for(int i = 0 ; i <N ; i++){
        int selected_bin = floor(arr[i]/binRange);
        bin[selected_bin]++;
    }
}



int main(int argc, char* argv[]){
    int thread_count = atoi(argv[1]);
    bool bad_critical_section = atoi(argv[2]);

    for(int i=0;i<N;i++)
      arr[i]=rand()%1000;

    bad_critical_section ? badloop(thread_count) : goodloop(thread_count);
    
    for (int j = 0 ; j <BIN_COUNT ; j++){
            printf("bin[%d] = %d\n", j , bin[j]);
    }

}