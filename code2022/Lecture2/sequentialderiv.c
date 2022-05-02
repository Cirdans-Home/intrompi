#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void firstderiv1D(int n, double a, double b, double *fx);
void firstderiv1D_vec(int n, double dx, double *f, double *fx);
double fun(double x);
double funprime( double x);

int main(int argc, char **argv){
  double a,b,dx,*f,*fx;
  int n;
  if(argc != 2){
    n = 20;
  }else{
    n = atoi(argv[1]);
  }
  a = 0.0;
  b = 1.0;
  f  = (double *) malloc(sizeof(double)*n);
  fx = (double *) malloc(sizeof(double)*n);
  // firstderiv1D(n, a, b, fx);
  dx = (b-a)/((double) n);
  for( int i = 0; i <= n; i++){
    f[i] = fun(a+((double) i)*dx);
  }
  firstderiv1D_vec(n, dx, f, fx);
  for(int i = 0; i < n; i++){
    printf("fx[%d] = %1.2f f'[%d] = %1.2f |fx - f'| = %1.1e\n",
      i,fx[i],i,funprime(a+((b-a)*i)/n),abs(fx[i]-
        funprime(a+((b-a)*((double) i))/((double) (n-1)))));
  }

  return 1;
}

void firstderiv1D(int n, double a, double b, double *fx){
  double dx = (b-a)/((double) n-1);
  double scale = 1.0/(2.0*dx);
  for (int i = 1; i < n-1; i++){
    fx[i] = (fun(a + (i+1)*dx) - fun(a + (i-1)*dx))*scale;
  }
  fx[0] = (-3.0*fun(a) + 4.0*fun(a + dx) - fun(a + 2.0*dx))*scale;
  fx[n-1] = (3.0*fun(a+ (n-1)*dx) - 4.0*fun(a + (n-2)*dx) + fun(a + (n-3)*dx))*scale;
  return;
}

void firstderiv1D_vec(int n, double dx, double *f, double *fx){
  double scale;
  scale = 1.0/(2.0*dx);
  for (int i = 1; i < n-1; i++){
    fx[i] = (f[i+1] - f[i-1])*scale;
  }
  fx[0] = (-3.0*f[0] + 4.0*f[1] - f[2])*scale;
  fx[n-1] = (3.0*f[n-1] - 4.0*f[n-2] + f[n-3])*scale;
  return;
}

double fun(double x){
  return(x*x + 1);
}

double funprime( double x){
  return(2*x);
}
