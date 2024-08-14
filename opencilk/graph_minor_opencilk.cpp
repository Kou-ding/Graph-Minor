#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include "include/mmio.h"
#include <atomic>

#define NUM_CLUSTERS 3 //number of clusters

// Helper function for atomic addition of double values
void atomicAdd(double* address, double val) {
    union {
        double f;
        uint64_t i;
    } oldval, newval;
    do {
        oldval.f = *address;
        newval.f = oldval.f + val;
    } while (!__sync_bool_compare_and_swap((uint64_t*)address, oldval.i, newval.i));
}

int main(int argc, char *argv[]) {
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;   
    int i, *I, *J, *Vector;
    double *val, **graphMinor;
    clock_t start_time, end_time;
    double cpu_time_used;

    // Start stopwatch
    start_time = clock();
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
        exit(1);
    } else { 
        if ((f = fopen(argv[1], "r")) == NULL) {
            exit(1);
        }    
    }

    if (mm_read_banner(f, &matcode) != 0) {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode)) {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
        exit(1);
    
    I = (int *) malloc(nz * sizeof(int));
    J = (int *) malloc(nz * sizeof(int));
    val = (double *) malloc(nz * sizeof(double));

    for (i=0; i<nz; i++) {
        fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        I[i]--;  /* adjust from 1-based to 0-based */
        J[i]--;
    }

    if (f !=stdin) fclose(f);
   
    Vector = (int *) malloc(M * sizeof(int));  
    if(Vector==NULL) {
        printf("Memory not available.");
        exit(1);
    }  

    srand(time(NULL));   
    std::atomic<int> vector_max(0);
    cilk_for(int x=0; x<M; x++) {  
        Vector[x] = (rand() % NUM_CLUSTERS);
        int current_max = vector_max.load();
        while(Vector[x] > current_max && !vector_max.compare_exchange_weak(current_max, Vector[x]));
    }
   
    graphMinor = (double **)malloc(M * sizeof(double *));
    if(graphMinor==NULL) {
        printf("Memory not available.");
        exit(1);
    }
    cilk_for (int x=0; x<M; x++) {
        graphMinor[x] = (double *)calloc(M, sizeof(double));
    }

    cilk_for(int x=0; x<nz; x++) {
        if(Vector[I[x]] != Vector[J[x]]) {
            atomicAdd(&graphMinor[Vector[I[x]]][Vector[J[x]]], val[x]);
        }
    }

    end_time = clock();

    // Free memory
    cilk_for(int x=0; x<M; x++) {
        free(graphMinor[x]);
    }
    free(graphMinor);
    free(I); 
    free(J); 
    free(val); 
    free(Vector);

    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", cpu_time_used);
    return 0;
}