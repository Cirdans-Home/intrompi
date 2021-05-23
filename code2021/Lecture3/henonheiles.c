#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double compute_energy(double p[2], double q[2], double omega[2]);
void kin_flow(double p[2], double q[2], double omega[2], double dt);
void pot_flow(double p[2], double q[2], double omega[2], double dt);
void leapfrog(double p[], double q[], double omega[], double dt);

int main(int argc, char **argv){

int myrank, size;
double input[7],*readdata;
int *intervals_for_process, grandtotal, *stride;

MPI_Init(&argc,&argv);

MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
MPI_Comm_size(MPI_COMM_WORLD,&size);

if(myrank == 0){
    FILE *ifp;
    char line[300];
    int numberoftests, i=0;

    ifp = fopen("henonheiles.inp","r");
    if (ifp == NULL) {
      fprintf(stderr, "Can't open input file henonheiles.inp!\n");
      MPI_Finalize();
      return 1;
    }
    fgets(line, 300, ifp);
    sscanf(line,"%d",&numberoftests);
    readdata = (double *) malloc( sizeof(double)*7*numberoftests );
    while(fgets(line, 300, ifp)!=NULL) {/*keep looping until NULL pointer */
        readdata[i]=atof(line); /* convert string to double */
        i++;
    }
    fclose(ifp);
}

/*We send to every processor the data relative to its simulation*/
MPI_Scatter(readdata,7,MPI_DOUBLE,input,7,MPI_DOUBLE,0,MPI_COMM_WORLD);

double omega[2],initialenergy,p0[2],q0[2],tmax,dt; // The seven inputs
double p[2],q[2],*delta_energy,energy_at_time_t,*every_delta_energy;
int numberofsteps;
omega[0] = input[0];
omega[1] = input[1];
initialenergy = input[2];
p0[1] = input[3];
q0[1] = input[4];
tmax = input[5];
dt = input[6];

numberofsteps = tmax/dt;	// It is defined as an integer, rounding up!

printf("Process %d is integrating the Henon-Heiles model with:\n"
	" omega = [%1.5f,%1.5f], E_0 = %1.5f, [p0,q0] = [%1.3f,%1.3f],"
	" until T = %1.1f with dt = %1.1e (%d intervals)\n",
    myrank,omega[0],omega[1],initialenergy,p0[1],q0[1],tmax,dt,numberofsteps);

delta_energy = (double *) malloc(sizeof(double)*(numberofsteps+1));

// We compute the first moment q0 from the value of the energy:
delta_energy[0] = initialenergy - omega[1]*(pow(p0[1],2) + pow(q0[1],2))/2.0
       + pow(p0[1],3)/3.0;
q0[0] = sqrt(2.0*delta_energy[0]/omega[0]);
p0[0] = 0;

for(int i=0; i<2; i++) {
    p[i] = p0[i];
    q[i] = q0[i];
}

for(int i=1; i<=numberofsteps; i++) {
    leapfrog(p, q, omega, dt);
    energy_at_time_t = compute_energy(p, q, omega);
    delta_energy[i] = energy_at_time_t - initialenergy;
}


/* We let all processors know how many intervals every process has computed,
i.e., what is the length of every delta_energy array */
numberofsteps++;
intervals_for_process = (int *) malloc(sizeof(int)*size);
MPI_Allgather(&numberofsteps,1,MPI_INT,
	intervals_for_process,1,MPI_INT,MPI_COMM_WORLD);
/* Now we need to gather all the (possibly different in length) arrays
containing the differente energies */
if (myrank == 0){
	/* Observe that we have allocated the memory for the receive only on the
	root processor.*/
	stride = (int *) malloc(sizeof(int)*size);
	for(int i = 0; i < size; i++){
		grandtotal += intervals_for_process[i];
		// Remember: recevbuf + stride[i]*extent[recevtype]!
		if(i==0){
			stride[i] = 0;
		}else{
			stride[i] = stride[i-1]+intervals_for_process[i-1];
		}
	}
	every_delta_energy = (double *) malloc(sizeof(double)*(grandtotal+size));

}
MPI_Gatherv(delta_energy,numberofsteps,MPI_DOUBLE,every_delta_energy,
	intervals_for_process,stride,MPI_DOUBLE,0,MPI_COMM_WORLD);


if (myrank == 0){
	/* Now we have all the errors on root processor, and we can print them to
	file for inspection */
	FILE *ofp;
	int glob_counter = 0;
	char filename[200];
	for(int i = 0; i < size;i++){
		sprintf(filename,"energy_process_%d.dat",i);
		ofp = fopen(filename,"w+");
		fprintf(ofp,"# omega = [%1.5f,%1.5f], E_0 = %1.5f, "
		"[p0,q0] = [%1.3f,%1.3f], until T = %1.1f with dt = %1.1e"
		" (%d intervals)\n"
		,readdata[i*7+0],readdata[i*7+1],readdata[i*7+2],
		readdata[i*7+3],readdata[i*7+4],readdata[i*7+5],
	    readdata[i*7+6],intervals_for_process[i]);
		for(int j=0; j < intervals_for_process[i]; j++){
			fprintf(ofp, "     %le      %le\n",
				j * readdata[i*7+6],
				every_delta_energy[glob_counter]);
			glob_counter++;
		}
		fclose(ofp);
	}
    free(readdata);
	free(every_delta_energy);
}
free(intervals_for_process);
MPI_Finalize();
return 0;
}

/* --------------------- Auxiliary Functions -------------------------------- */

double compute_energy(double p[2], double q[2], double omega[2]){
  int i;
  double ris = 0;
  for(i=0; i<2; i++)
    ris += omega[i] * ( pow(p[i],2) + pow(q[i],2) ) / 2;
  ris += p[1] * ( pow(p[0],2) - pow(p[1],2) / 3 );
  return ris;
}
void kin_flow(double p[2], double q[2], double omega[2], double dt){
  p[0] += omega[0] * q[0] * 0.5 * dt;
  p[1] += omega[1] * q[1] * 0.5 * dt;
}
void pot_flow(double p[2], double q[2], double omega[2], double dt){
  q[0] -= ( omega[0] * p[0] + 2 * p[0] * p[1] ) * dt;
  q[1] -= ( omega[1] * p[1] + pow(p[0],2) - pow(p[1],2) ) * dt;
}
void leapfrog(double p[2], double q[2], double omega[2], double dt){
  kin_flow(p, q, omega, dt);
  pot_flow(p, q, omega, dt);
  kin_flow(p, q, omega, dt);
}
