#include <iostream>
using namespace std;
#include <omp.h>
#include <vector>
#include <fstream>
#include <cmath>

#define CACHE_LINE 64
#define INT_SIZE 4


enum {Nouter=10000,Ninner=10000};
int arr[Nouter];

int runloop(int i, bool worse) {
	int n = worse ? (int)(pow(i,4.0)/pow(Nouter,3.0)) : i;
	int sum=0;
	for (int j=0;j<n;j++)
		sum+=i^j;
	return sum;
}



int main(int argc, char* argv[]){
    int thread_count = 8;
    
    bool worsen_load_balancing = 1;
    arr[0]=0;
    cout << boolalpha << worsen_load_balancing << "\n";

    // time array that keeps count of the time spent by each thread
    double threadTime[thread_count];
    for (int k = 0 ; k < thread_count ; k++){
      threadTime[k] = 0.0;
    } 

    omp_set_num_threads(thread_count);
    // #pragma omp parallel for schedule(static,1)
    #pragma omp parallel for 
    for (unsigned int i=0;i<Nouter;i++) {
    int my_rank = omp_get_thread_num();
    double start_time = omp_get_wtime();
		arr[i]=runloop(i,worsen_load_balancing);
    // cout << my_rank <<"\n";
    threadTime[my_rank] += omp_get_wtime() - start_time;
    }


    for (int k = 0 ; k < thread_count ; k++){
      cout << "Time taken by Thread" << k +1 << "is -> " << threadTime[k] << " sec" << "\n";
    }
   cout << arr[9999]<<"\n";
   return arr[0];


}
