CC =g++
CFLAGS = -Wall -O3 -std=c++11 -fopenmp
LFLAGS = -lgomp

all: openmp

openmp: graph_minor_openmp.cpp mmio.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

clean:
	rm -f openmp
