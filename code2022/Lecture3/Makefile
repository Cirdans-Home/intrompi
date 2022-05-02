MPICC = mpicc			#The wrapper for the compiler
CFLAGS += -g			#Useful for debug symbols
LDLIBS += -lm

all: contiguousdatatype vectordatatype subarrayexample \
	subarray2Darray timers henonheiles

contiguousdatatype: contiguousdatatype.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

vectordatatype: vectordatatype.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

subarrayexample: subarrayexample.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

subarray2Darray: subarray2Darray.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

henonheiles: henonheiles.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

timers: timers.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

clean:
	rm -f contiguousdatatype vectordatatype subarrayexample \
		subarray2Darray timers henonheiles
