#include <iostream>
#include <omp.h>
#include <vector>
#include <fstream>
#include <cmath>


using namespace std;

#define N 10000


int arr[N];
int global_max=-1;


int RandomNumberGenerator(int val){

    for(int j = 0 ; j < 1000000 ; j++){
        val = rand()%1000;
    }
    return (int)val;
}

void badloop() {


    cout << "This is Bad version "<< "\n";
	#pragma omp parallel for
    for (int i = 0 ; i < N; ++i ) 
    { 
        #pragma omp critical
        {
            if (arr[i] > global_max) global_max = RandomNumberGenerator(arr[i]);
        } 
    }

}

void goodloop() {

    cout << "This is good version "<< "\n";

	#pragma omp parallel for
    for (int i = 0 ; i < N; ++i ) 
    { 
        // #pragma omp flush(global_max)
        if (arr[i] > global_max)
        {
            #pragma omp critical
            {
                global_max = RandomNumberGenerator(arr[i]);
            }
        }
    }
}



int main(int argc, char* argv[]){
    int thread_count = atoi(argv[1]);
    bool bad_critical_section = atoi(argv[2]);

    for(int i=0;i<N;i++)
      arr[i]=rand()%1000;


    bad_critical_section ? badloop() : goodloop();
    
    cout << "The global maximum we found :" << global_max <<"\n";
    return global_max;

}
