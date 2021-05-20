MPICC = mpicc			#The wrapper for the compiler
CC = gcc
CFLAGS += -g			#Useful for debug symbols
LDLIBS += -lm

all: parallelderiv midpointintegral montecarlostyle

parallelderiv: parallelderiv.c
	$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

midpointintegral: midpointintegral.c
		$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

montecarlostyle: montecarlostyle.c
		$(MPICC) $(CFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

clean:
	rm -f parallelderiv midpointintegral montecarlostyle
