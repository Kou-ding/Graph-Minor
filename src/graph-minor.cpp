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
    double *val;

    if (argc < 2){
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	}
    else{ 
        if ((f = fopen(argv[1], "r")) == NULL) 
            exit(1);
    }

    if (mm_read_banner(f, &matcode) != 0){
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }


    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */
    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && 
            mm_is_sparse(matcode) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }


    /* find out size of sparse matrix .... */
    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
        exit(1);


    /* reseve memory for matrices */
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

    printf("Matrix details:\n");
    mm_write_banner(stdout, matcode);

    printf("Matrix rows, columns, nnz:\n");
    mm_write_mtx_crd_size(stdout, M, N, nz);
    /*
    printf("Row | Column | Value\n");
    for (i=0; i<nz; i++){
        fprintf(stdout, "%d %d %20.19g\n", I[i], J[i], val[i]);
    }
    */
    Vector = (int *) malloc(M * sizeof(int));    

    /*create random vector Vector*/
    srand(time(NULL));   
    for(i=0;i<M;i++){  
        Vector[i]=rand()%8;
        printf("%d\n", Vector[i]);
    }

	return 0;
}

