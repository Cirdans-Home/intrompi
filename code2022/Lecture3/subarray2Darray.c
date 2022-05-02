#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void printarr(int **data, int n, char *str);
int **allocarray(int n);

int main(int argc, char **argv) {

    /* array sizes */
    // We define first the constant size of the 2Darray and of the 2Dsubarray
    const int bigsize = ;
    const int subsize = ;

    /* communications parameters */
    // We are going to perform just a single communication, so chose a value for
    // the tag:
    const int tag  = ;

    int rank, size;
    MPI_Status status;
    // Init here the MPI environment and get the rank and the global size of the
    // communication
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check that there are at least 2 processors, otherwise
    // 1) If you are on rank == 0 put an error message
    // 2) finalize the MPI environment,
    // 3) return 1; (ERROR!)
    if (size < 2) {
        if (rank == 0)
            fprintf(stderr,"%s: Needs at least %d  processors.\n", argv[0], 2);
        MPI_Finalize();
        return 1;
    }


    if (rank == 0) {
        // Rank 0 allocates a bigsize x bigsize 2D array:
        int **bigarray = allocarray(bigsize);
        // We put some numbers into it by doing a double for cycle
        for (int i=0; i<bigsize; i++)
            for (int j=0; j<bigsize; j++)
                bigarray[i][j] = i*bigsize+j;
        // We print on screen the array we have allocated:
        printarr(bigarray, bigsize, " Sender: Big array ");

        // Now the core of the program, we define the new datatype
        MPI_Datatype mysubarray;
        // We choose the coordinate from which we start the communication:
        int starts[ ] = {};
        // The size of the square subarray to communicate:
        int subsizes[ ]  = {};
        // The size of the big square array:
        int bigsizes[ ]  = {};
        // We create the subarray type:
        MPI_Type_create_subarray();
        // We commit it:
        MPI_Type_commit();
        // We send the subarray to process 1:
        MPI_Send();
        // We deallocate the type:
        MPI_Type_free();
        // free the memory allocated for the bigarray:
        free(bigarray[0]);
        free(bigarray);

    } else if (rank == 1) {
        // Now we are rank 1: we first allocate the array for receiving the
        // message from rank 0 (it has to be a square array of size subsize^2):
        int **subarray = allocarray(subsize);
        // with a double for loop we put to zero the entries of the receiveing
        // array:
        for (int i=0; i<subsize; i++)
            for (int j=0; j<subsize; j++)
                subarray[i][j] = 0;
        // We receive the data in the subarray we have allocated
        MPI_Recv();
        // We print what we have received:
        printarr(subarray, subsize, " Receiver: Subarray -- after receive");
        // We free the memory for subarray
        free(subarray[0]);
        free(subarray);
    }
    // We finalize the MPI environment and return 0: SUCCESS!
    MPI_Finalize();
    return 0;
}

void printarr(int **array, int n, char *str) {
    printf("-- %s --\n", str);
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }
}

int **allocarray(int n) {
    int *rows = malloc(n*n*sizeof(int));
    int **arr2D = malloc(n*sizeof(int *));
    for (int i=0; i<n; i++)
        arr2D[i] = &(rows[i*n]);

    return arr2D;
}
