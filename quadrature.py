"""
Computing pi with parallel quadrature formula
"""
import numpy
from mpi4py import MPI
import sys

comm = MPI.COMM_WORLD
mynode = comm.Get_rank()
totalnodes = comm.Get_size()

def fun(x):
    return (4.0/(1.0 + x*x))

truepi = 3.141592653589793238462643
dest = 0
pi = numpy.zeros(1)

# Initialize value of n only if this is rank 0
# we are not checking the input, if the user does something that
# has no meaning we are going to fail badly!
if mynode == 0:
    if len(sys.argv) == 1:
        n = numpy.full(1, 20, dtype=int) # default value
    else:
        n = numpy.full(1,int(sys.argv[1]),dtype=int)
else:
    n = numpy.zeros(1, dtype=int)

# Broadcast n to all processes
comm.Bcast(n, root=0)

# Compute local integral
my_pi = numpy.zeros(1)
h = 1.0/(n*totalnodes)
for i in numpy.arange(1+mynode*n,n*(mynode+1)+1):
    x = h*(i - 0.5)
    my_pi = my_pi + fun(x)    
my_pi = h*my_pi

# Send partition back to root process:
comm.Reduce(my_pi, pi, MPI.SUM, dest)

# Only print the result in process 0
if mynode == 0:
    print('The Integral Sum =', pi[0]," The Error is ",numpy.abs(pi[0]-truepi))
