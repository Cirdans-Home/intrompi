#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double f( double x);

int main( int argc, char *argv[]){
int n, mynode, totalnodes, i;
double PI25DT = 3.141592653589793238462643;
double mypi, pi, h, sum, x;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&totalnodes);
MPI_Comm_rank(MPI_COMM_WORLD,&mynode);

if(mynode == 0){
  if(argc != 2){
   n = 20;
  }else{
   n = atoi(argv[1]);
  }
}
MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
h   = 1.0 / ((double) n*totalnodes);
sum = 0.0;
for (i = 1+mynode*n; i <= n*(mynode+1); i++){
 x = h * ((double)i - 0.5);
 sum += f(x);
}
mypi = h*sum;
MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
if (mynode == 0){
 printf("pi is approximately %.16f, Error is %e\n",pi, fabs(pi - PI25DT));
}
MPI_Finalize();
return 0;
}

double f( double x )
{
    return (4.0 / (1.0 + x*x));
}
