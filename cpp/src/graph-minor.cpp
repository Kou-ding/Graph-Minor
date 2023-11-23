#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/mmio.h"

int main(int argc, char *argv[]){

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

    /************************/
    /* now write out matrix */
    /************************/
    /*
    //print initial matrix market file information
    printf("Matrix details:\n");
    mm_write_banner(stdout, matcode);

    //print dimensions and number of non-zeros (nnz)
    printf("Matrix rows, columns, nnz:\n");
    mm_write_mtx_crd_size(stdout, M, N, nz);

    //print the rows, columns and values
    printf("Row | Column | Value\n");
    for (i=0; i<nz; i++){
        fprintf(stdout, " %d %d %20.19g\n", I[i], J[i], val[i]);
    }
    */
    //Mapping Vector
    Vector = (int *) malloc(M * sizeof(int));  
    if(Vector==NULL){
        printf("Memory not available.");
        exit(1);
    }  
    //int example_clustering[]={0,1,0,2,1};
    //Vector=example_clustering;
    
    //create random vector Vector
    srand(time(NULL));   
    int vector_max=0;
    #pragma omp parallel for
    for(int x=0;x<M;x++){  
        Vector[x]=(rand()%3);
        #pragma omp critical
        if (Vector[x]>vector_max){
            vector_max=Vector[x];
        }
        //printf("Vector[%d] = %d\n",x,Vector[x]); //if we wanted to know the insides of the array Vector[]
    }
   
    //Clustering
    graphMinor = (double **)malloc(M * sizeof(double *));
    //a double pointer points at the rows of the 2d matrix created 
    if(graphMinor==NULL){
        printf("Memory not available.");
        exit(1);
    }
    for (int x=0;x<M;x++) {
        graphMinor[x]=(double *)malloc(M * sizeof(double));
    }

    //initialize graph minor
    for(int x=0;x<=vector_max;x++){
        for(int y=0;y<=vector_max;y++){
            graphMinor[x][y]=0;
        }
    }

    //create connections between clusters
    for(int x=0;x<nz;x++){
        if(Vector[I[x]] != Vector[J[x]]){
            graphMinor[Vector[I[x]]][Vector[J[x]]]+=val[x];
            /*//Extra code for debugging
            printf("x: %d,\nI[x]: %d,\nJ[x]: %d,\nval[x]: %.0f,\n",x,I[x],J[x],val[x]);
            printf("Vector[I[x]]: %d,\nVector[J[x]]: %d,\ngm: %.0f\n\n",Vector[I[x]],Vector[J[x]],graphMinor[Vector[I[x]]][Vector[J[x]]]);
            */
        }
    }
    //stop stopwatch
    end_time = clock();
    

    //Print the Graph minor
    /*
    printf("\nGraphMinor:\n");
    for(int x=0;x<=vector_max;x++){
        printf("|");
        for(int y=0;y<=vector_max;y++){
            printf(" [%d][%d]= %.3f |",x,y,graphMinor[x][y]);
        }
        printf("\n");
    }
    */


    //free memory
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

