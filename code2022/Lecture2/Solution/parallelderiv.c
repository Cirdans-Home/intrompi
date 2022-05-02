#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

void firstderiv1Dp_vec(int n, double dx, double *f,double *fx, int mynode, int totalnodes);

double fun(double x);
double funprime( double x);

int main(int argc, char **argv){
  double globala,globalb,a,b,dx,*f,*fx, error;
  int n, mynode, totalnodes;
  MPI_Status status;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &mynode );
  MPI_Comm_size( MPI_COMM_WORLD, &totalnodes );

  n = 20;

  globala = 0;
  globalb = 1;

  a = globala + ((double) mynode)*(globalb - globala)/( (double) totalnodes);
  b = globala + ((double) mynode+1)*(globalb - globala)/( (double) totalnodes);

  f  = (double *) malloc(sizeof(double)*(n));
  fx = (double *) malloc(sizeof(double)*(n));

  dx = (b-a)/((double) n);
  for( int i = 0; i < n; i++){
    f[i] = fun(a+((double) i)*dx);
  }
  firstderiv1Dp_vec( n, dx, f, fx, mynode, totalnodes);

  error = 0.0;
  for(int i = 0; i < n; i++){
    error += pow( fx[i]-funprime(a+((b-a)*((double) i))/((double) n)),2.0);
  }
  error = sqrt(dx*error);
  printf("Node %d ||f' - fx||_2 = %e\n",mynode,error);


  free(f);
  free(fx);
  MPI_Finalize();

  return 0;
}


void firstderiv1Dp_vec(int n, double dx, double *f,double *fx, int mynode, int totalnodes){
double scale = 1.0/(2.0*dx);
double mpitemp;
MPI_Status status;

if(mynode == 0){
 fx[0] = (-3.0*f[0] + 4.0*f[1] - f[2])*scale;
}
if(mynode == (totalnodes-1)){
 fx[n-1] = (3.0*f[n-1] - 4.0*f[n-2] + f[n-3])*scale;
}
for(int i=1;i<n-1;i++){
 fx[i] = (f[i+1]-f[i-1])*scale;
}
if(mynode == 0){
   mpitemp = f[n-1];
   MPI_Send(&mpitemp,1,MPI_DOUBLE,1,1,MPI_COMM_WORLD);
   MPI_Recv(&mpitemp,1,MPI_DOUBLE,1,1,MPI_COMM_WORLD,&status);
   fx[n-1] = (mpitemp - f[n-2])*scale;
}
else if(mynode == (totalnodes-1)){
  MPI_Recv(&mpitemp,1,MPI_DOUBLE,mynode-1,1,MPI_COMM_WORLD, &status);
  fx[0] = (f[1]-mpitemp)*scale;
  mpitemp = f[0];
  MPI_Send(&mpitemp,1,MPI_DOUBLE,mynode-1,1,MPI_COMM_WORLD);
}
else{
 MPI_Recv(&mpitemp,1,MPI_DOUBLE,mynode-1,1,MPI_COMM_WORLD, &status);
 fx[0] = (f[1]-mpitemp)*scale;
 mpitemp = f[0];
 MPI_Send(&mpitemp,1,MPI_DOUBLE,mynode-1,1,MPI_COMM_WORLD);
 mpitemp = f[n-1];
 MPI_Send(&mpitemp,1,MPI_DOUBLE,mynode+1,1,MPI_COMM_WORLD);
 MPI_Recv(&mpitemp,1,MPI_DOUBLE,mynode+1,1,MPI_COMM_WORLD, &status);
 fx[n-1] = (mpitemp-f[n-2])*scale;
}
return;
}

double fun(double x){
  return(sin(x));
}

double funprime( double x){
  return(cos(x));
}
