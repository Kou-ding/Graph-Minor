CC = /home/kou/coding/Graph-Minor/opencilk_compiler/opencilk-2.1.0-x86_64-linux-gnu-ubuntu-22.04/bin/clang++
CFLAGS = -Wall -O3 -std=c++11 -fopencilk
LFLAGS = -fopencilk

all: cilk

cilk: graph_minor_opencilk.cpp mmio.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

clean:
	rm -f cilk
