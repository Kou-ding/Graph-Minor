#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/mmio.h"
#include <omp.h>

#define NUM_CLUSTERS 3 //number of clusters

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
        fprintf(stderr, "Usage: %s [matrix-market-filename]\n", argv[0]);
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

    // This is how one can screen matrix types if their application 
    //  only supports a subset of the Matrix Market data types.
    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode)) {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    // find out size of sparse matrix .... 
    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) != 0)
        exit(1);

    // reseve memory for matrices 
    I = (int *) malloc(nz * sizeof(int));
    J = (int *) malloc(nz * sizeof(int));
    val = (double *) malloc(nz * sizeof(double));

    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */
    for (i = 0; i < nz; i++) {
        fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        I[i]--;  // adjust from 1-based to 0-based
        J[i]--;
    }

    if (f != stdin) fclose(f);
    
    //Mapping Vector
    Vector = (int *) malloc(M * sizeof(int));
    if (Vector == NULL) {
        printf("Memory not available.");
        exit(1);
    }

    // Assign a random number to each node
    // This number will be the cluster that the node belongs to
    srand(time(NULL));
    int vector_max = 0;
    // Parallelize the loop
    #pragma omp parallel
    {
        unsigned int seed = omp_get_thread_num();
        // A reduction operation will be performed on the variable vector_max. 
        // Reduction operator : max.
        // Each thread will keep track of the maximum value of vector_max within its subset of iterations.
        #pragma omp for reduction(max:vector_max)
        for (int x = 0; x < M; x++) {
            Vector[x] = rand_r(&seed) % NUM_CLUSTERS;
            if (Vector[x] > vector_max) {
                vector_max = Vector[x];
            }
        }
    }

    //Clustering
    graphMinor = (double **) malloc((vector_max + 1) * sizeof(double *));
    if (graphMinor == NULL) {
        printf("Memory not available.");
        exit(1);
    }
    for (int x = 0; x <= vector_max; x++) {
        graphMinor[x] = (double *) calloc(vector_max + 1, sizeof(double));
    }

    // Initialize graph minor
    for(int x=0;x<=vector_max;x++){
        for(int y=0;y<=vector_max;y++){
            graphMinor[x][y]=0;
        }
    }
    // Create connections between clusters
    #pragma omp parallel for
    for (int x = 0; x < nz; x++) {
        if (Vector[I[x]] != Vector[J[x]]) {
            // Prevent race condition
            // by ensuring that this section of code is executed atomically
            #pragma omp atomic 
            graphMinor[Vector[I[x]]][Vector[J[x]]] += val[x];
        }
    }

    // Stop stopwatch
    end_time = clock();

    // Free memory
    for (int x = 0; x <= vector_max; x++) {
        free(graphMinor[x]);
    }
    free(graphMinor);
    free(I);
    free(J);
    free(val);
    free(Vector);

    // Calculate the CPU time used
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", cpu_time_used);
    return 0;
}