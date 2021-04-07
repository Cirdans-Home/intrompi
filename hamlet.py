"""
Hello (parallel) world!
"""
from mpi4py import MPI

comm = MPI.COMM_WORLD 
rank = comm.Get_rank() 
size = comm.Get_size() 

print("Hello world! I'm process ",rank," of ",size)
