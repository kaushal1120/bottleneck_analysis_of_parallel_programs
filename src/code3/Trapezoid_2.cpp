#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Trap(double a, double b, int n, double*  result_p);

int main(int argc, char*  argv[]) {

    double global_result = 0.0;
    double  a, b;
    int   n;
    int  thread_count;

    thread_count = 4;
    double threadArray[thread_count];

    for (int j = 0; j < thread_count; j++){
        threadArray[j] = 0.0;
    }

    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", &a, &b, &n);

    #pragma omp parallel num_threads(thread_count)
    {
        int rank = omp_get_thread_num();
        Trap(a, b, n, &threadArray[rank]);
    }
    

    for (int j = 0; j < thread_count ; j++){
        global_result += threadArray[j];
    }
    printf("With n = %d trapezoids, our estimatenn", n);
    printf("of the integral from %f to %f = %.14e\n",
    a, b, global_result);

    return 0;

}  

int f (int num){
    return num*num;
}

void Trap(double a, double b, int n, double*  result_p) {

    double  h, x, my_result;
    double  local_a, local_b;
    int  i, local_n;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    h = (b-a)/n;
    local_n = n/thread_count;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n*h;

    my_result = (f(local_a) + f(local_b))/2.0;
    for (i = 1; i <= local_n-1; i++) {
    x = local_a + i*h;
    my_result += f(x);
    }

    my_result = my_result*h;
    // #  pragma omp critical
    *result_p += my_result;

}  /*  Trap  */