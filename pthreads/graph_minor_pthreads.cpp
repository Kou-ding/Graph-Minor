#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "include/mmio.h"

#define NUM_CLUSTERS 3 //number of clusters
#define NUM_THREADS 4 //number of threads to use

typedef struct {
    int start;
    int end;
    int *Vector;
    int M;
} VectorInitArgs;

typedef struct {
    int start;
    int end;
    int *I;
    int *J;
    double *val;
    int *Vector;
    double **graphMinor;
} GraphMinorArgs;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* initializeVector(void* args) {
    VectorInitArgs *threadArgs = (VectorInitArgs*)args;
    for(int x = threadArgs->start; x < threadArgs->end; x++) {
        threadArgs->Vector[x] = (rand() % NUM_CLUSTERS);
    }
    return NULL;
}

void* createGraphMinor(void* args) {
    GraphMinorArgs *threadArgs = (GraphMinorArgs*)args;
    for(int x = threadArgs->start; x < threadArgs->end; x++) {
        if(threadArgs->Vector[threadArgs->I[x]] != threadArgs->Vector[threadArgs->J[x]]) {
            pthread_mutex_lock(&mutex);
            threadArgs->graphMinor[threadArgs->Vector[threadArgs->I[x]]][threadArgs->Vector[threadArgs->J[x]]] += threadArgs->val[x];
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
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
    
    if (argc < 2){
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	}
    else{ 
        if ((f = fopen(argv[1], "r")) == NULL){
            exit(1);
        }    
    }

    if (mm_read_banner(f, &matcode) != 0){
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }


    // This is how one can screen matrix types if their application 
    //  only supports a subset of the Matrix Market data types.      
    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode) ){
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    // find out size of sparse matrix .... 
    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
        exit(1);
    
    // reseve memory for matrices 
    I = (int *) malloc(nz * sizeof(int));
    J = (int *) malloc(nz * sizeof(int));
    val = (double *) malloc(nz * sizeof(double));

    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */
    for (i=0; i<nz; i++)
    {
        fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        I[i]--;  /* adjust from 1-based to 0-based */
        J[i]--;
    }

    if (f !=stdin) fclose(f);

    // Mapping Vector
    Vector = (int *) malloc(M * sizeof(int));  
    if(Vector==NULL){
        printf("Memory not available.");
        exit(1);
    }  

    // Initialize Vector using threads
    pthread_t vector_threads[NUM_THREADS];
    VectorInitArgs vector_args[NUM_THREADS];
    int chunk_size = M / NUM_THREADS;
    
    srand(time(NULL));
    for(int i = 0; i < NUM_THREADS; i++) {
        vector_args[i].start = i * chunk_size;
        vector_args[i].end = (i == NUM_THREADS - 1) ? M : (i + 1) * chunk_size;
        vector_args[i].Vector = Vector;
        vector_args[i].M = M;
        pthread_create(&vector_threads[i], NULL, initializeVector, (void*)&vector_args[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(vector_threads[i], NULL);
    }

    // Find vector_max
    int vector_max = 0;
    for(int x = 0; x < M; x++) {
        if (Vector[x] > vector_max) {
            vector_max = Vector[x];
        }
    }

    // Clustering
    graphMinor = (double **)malloc(M * sizeof(double *));
    if(graphMinor==NULL){
        printf("Memory not available.");
        exit(1);
    }
    for (int x = 0; x <= M; x++) {
        graphMinor[x] = (double *)calloc(M , sizeof(double));
    }

    // Create connections between clusters using threads
    pthread_t graph_threads[NUM_THREADS];
    GraphMinorArgs graph_args[NUM_THREADS];
    chunk_size = nz / NUM_THREADS;
    
    for(int i = 0; i < NUM_THREADS; i++) {
        graph_args[i].start = i * chunk_size;
        graph_args[i].end = (i == NUM_THREADS - 1) ? nz : (i + 1) * chunk_size;
        graph_args[i].I = I;
        graph_args[i].J = J;
        graph_args[i].val = val;
        graph_args[i].Vector = Vector;
        graph_args[i].graphMinor = graphMinor;
        pthread_create(&graph_threads[i], NULL, createGraphMinor, (void*)&graph_args[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(graph_threads[i], NULL);
    }
    // Stop stopwatch
    end_time = clock();

    // Free memory
    for(int x=0;x<M;x++){
        free(graphMinor[x]); //free each row of double pointer
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