#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, i;
    MPI_Datatype myvector, my3int;
    int buffer[24];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Type_contiguous(3, MPI_INT, &my3int);
    MPI_Type_commit(&my3int);
    MPI_Type_vector(3, 2, 3, my3int, &myvector);
    MPI_Type_commit(&myvector);

    if (rank == 0){
        for (i=0; i<24; i++)
            buffer[i] = i;
        MPI_Send(buffer, 1, myvector, 1, 666, MPI_COMM_WORLD);
    }else if(rank == 1){
        for (i=0; i<24; i++)
            buffer[i] = -1;
        MPI_Recv(buffer, 1, myvector, 0, 666, MPI_COMM_WORLD, &status);
        for (i=0; i<24; i++)
            printf("buffer[%d] = %d\n", i, buffer[i]);
        fflush(stdout);
    }
    MPI_Type_free(&my3int);
    MPI_Type_free(&myvector);
    MPI_Finalize();
    return 0;
}
