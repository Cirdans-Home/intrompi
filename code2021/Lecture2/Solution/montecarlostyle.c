#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

int main( int argc, char *argv[]){
 int n, mynode, totalnodes, i;
 double PI25DT = 3.141592653589793238462643;
 double mypi, pi, h;
 double x, y, x1, x2, y1, y2;
 int SqPoints, CiPoints, my_SqPoints, my_CiPoints;
 unsigned int seed = (unsigned) time(NULL);

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

 h  = 2.0 / (double) totalnodes ;
 x1 = -1.0 + mynode * h;
 x2 = x1 + h;
 y1 = -1.0;
 y2 = 1.0;
 my_SqPoints  = 0;
 my_CiPoints  = 0;

 for (i = 1; i <= n; i++)
 {
    srand(seed + mynode);
    x = rand_r(&seed); x = x / RAND_MAX; x = x1 + x * (x2 - x1);
    y = rand_r(&seed); y = y / RAND_MAX; y = y1 + y * (y2 - y1);
    my_SqPoints++;
    if ( ( x*x + y*y ) <= 1.0 ) my_CiPoints++;
}
SqPoints = 0;
CiPoints = 0;
MPI_Reduce(&my_SqPoints, &SqPoints, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
MPI_Reduce(&my_CiPoints, &CiPoints, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

if (mynode == 0)
{
   pi = 4.0 * (double)CiPoints / (double)SqPoints;
   printf("Pi is approximately %.16f = 4.0 %d/%d, Error is %e (h = %e)\n",pi,CiPoints,SqPoints,fabs(pi - PI25DT),1.0/sqrt( (double) SqPoints) );
}

MPI_Finalize();

return 0;
}
