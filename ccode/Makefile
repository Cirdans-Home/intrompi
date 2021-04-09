MPICC = mpicc			#The wrapper for the compiler
CFLAGS += -g			#Useful for debug symbols
LDLIBS=-lm -ldl

all: helloworld hamlet

helloworld: helloworld.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

hamlet: hamlet.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@
    
easysendrecv: easysendrecv.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

nonblockingsendrecv: nonblockingsendrecv.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

sequentialderiv: sequentialderiv.c
	$(CC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

parallelderiv: parallelderiv.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

clean:
	rm -f helloworld hamlet easysendrecv nonblockingsendrecv sequentialderiv parallelderiv
