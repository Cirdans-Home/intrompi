"""
Computing pi with parallel quadrature formula
"""
import numpy
from mpi4py import MPI
import sys
import random

comm = MPI.COMM_WORLD
mynode = comm.Get_rank()
totalnodes = comm.Get_size()

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

# Compute local quantities
h  = 2.0 / totalnodes
x1 = -1.0 + mynode * h
x2 = x1 + h
y1 = -1.0
y2 = 1.0
my_SqPoints  = numpy.zeros(1, dtype=int)
my_CiPoints  = numpy.zeros(1, dtype=int)

for i in numpy.arange(1,n+1,totalnodes):
    x = random.random()
    x = x1 + x*(x2-x1)
    y = random.random()
    y = y1 + y*(y2-y1)
    my_SqPoints = my_SqPoints + 1
    if ( x**2 + y**2 <= 1.0 ):
        my_CiPoints = my_CiPoints + 1
    
SqPoints = numpy.zeros(1, dtype=int)
CiPoints = numpy.zeros(1, dtype=int)

# Send back to the root process the data
comm.Reduce(my_SqPoints, SqPoints, MPI.SUM, dest)
comm.Reduce(my_CiPoints, CiPoints, MPI.SUM, dest)

# Only print the result in process 0
if mynode == 0:
    pi = 4.0*float(CiPoints)/float(SqPoints)
    print('The Integral Sum =', pi," The Error is ",numpy.abs(pi-truepi))
