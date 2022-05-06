#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void transpose(double *A, double *B, int n) {
    int i,j;
    for(i=0; i<n; i++) {
	for(j=0; j<n; j++) {
            B[j*n+i] = A[i*n+j];
	}
    }
}

void mm_1(double *A, double *B, double *C, int n) 
{	
    #pragma omp parallel num_threads(8)
    {
	int i, j, k;
	#pragma omp for
	for (i = 0; i < n; i++) { 
	    for (j = 0; j < n; j++) {
	        double dot  = 0;
	        for (k = 0; k < n; k++) {
	            dot += A[i*n+k]*B[k*n+j];
		} 
	        C[i*n+j ] = dot;
            }
	}	
    }
}

void mm_2(double *A, double *B, double *C, int n) 
{	
    double *B2;
    B2 = (double*)malloc(sizeof(double)*n*n);
    transpose(B,B2, n);
    #pragma omp parallel num_threads(8)
    {
        int i, j, k;
	#pragma omp for
	for (i = 0; i < n; i++) { 
	    for (j = 0; j < n; j++) {
		double dot  = 0;
		for (k = 0; k < n; k++) {
		    dot += A[i*n+k]*B2[j*n+k];
		} 
		C[i*n+j] = dot;
	    }
	}	
    }
    free(B2);
}

int main(int argc, char* argv[]) {
    int i, n;
    double *A, *B, *C, dtime;

    n  = 512;
    A = (double*)malloc(sizeof(double)*n*n);
    B = (double*)malloc(sizeof(double)*n*n);
    C = (double*)malloc(sizeof(double)*n*n);
    for(i=0; i<n*n; i++) { 
        A[i] = rand()/RAND_MAX; B[i] = rand()/RAND_MAX;
    }

    if(atoi(argv[1]) == 1){
        dtime = omp_get_wtime();
   	mm_1(A,B,C, n);
	dtime = omp_get_wtime() - dtime;
        printf("%f\n", dtime);
    }
    else{
        dtime = omp_get_wtime();
        mm_2(A,B,C, n);
        dtime = omp_get_wtime() - dtime;
        printf("%f\n", dtime);
    }
    return 0;
}
