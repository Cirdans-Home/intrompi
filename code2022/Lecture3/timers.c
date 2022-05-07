#include <stdio.h>
#include <mpi.h>


int main(int argc,char **argv){

int myrank,size;
double tic,toc;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
MPI_Comm_size(MPI_COMM_WORLD,&size);

MPI_Barrier(MPI_COMM_WORLD);
tic = MPI_Wtime();
printf("Hello, world.  I am %d of %d\n", myrank, size);
MPI_Barrier(MPI_COMM_WORLD);
toc = MPI_Wtime();
printf("Hello, world.  I am %d of %d and my time is %e\n", myrank, size,toc-tic);
fflush(stdout);
MPI_Finalize();
return 0;
}
