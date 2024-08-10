# $^ (all prerequisites): refers to whatever is after the ":"
# $@ (target): refers to whatever is before the ":"
# $< (first prerequisite): refers to the first thing that is after the ":"

CC = g++
CFLAGS = -std=c++11 -Wall -pthread

# all is the default action in makefiles
all: graph_minor_pthreads

graph_minor_pthreads: graph_minor_pthreads.cpp mmio.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f graph_minor_pthreads