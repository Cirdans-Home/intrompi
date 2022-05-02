#include "mpi.h"
#include <string.h>
#include <stdio.h>
int main( int argc, char **argv){
 char message[20];
 int myrank;
 MPI_Status status;
 MPI_Datatype mystring;
 MPI_Init( &argc, &argv );
 MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

 MPI_Type_contiguous(20,MPI_CHAR,&mystring);
 MPI_Type_commit(&mystring);

 if (myrank == 0){  /* code for process zero */
  strcpy(message,"Hello, there");
  MPI_Send(message, 1, mystring, 1, 99, MPI_COMM_WORLD);
 }
 else if (myrank == 1){ /* code for process one */
  MPI_Recv(message, 1, mystring, 0, 99, MPI_COMM_WORLD, &status);
  printf("received :%s:\n", message);
 }
 MPI_Type_free(&mystring);
 MPI_Finalize();
 return 0;
}
