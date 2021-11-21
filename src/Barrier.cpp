#include <unistd.h>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>

#define THREADS 8

/* the function called for each thread */
void worker() {
  /* get our thread id */
  int id = omp_get_thread_num();

  /* we start to work */
  printf("Thread %d starting!\n", id);

  /* simulate the threads taking slightly different amounts of time by sleeping
   * for our thread id seconds */
  sleep(id);
  printf("Thread %d is done its work!\n", id);


  /* a barrier */
  #pragma omp barrier


  printf("Thread %d is past the barrier!\n", id); 
}

int main() {
  /* have all the threads run worker */
  # pragma omp parallel num_threads(THREADS)
    worker();

  return 0;
}