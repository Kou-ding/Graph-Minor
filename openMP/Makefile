CC := $(shell command -v clang 2> /dev/null)
ifeq ($(CC),)
    CC := gcc
endif
CFLAGS = -Wall -std=c++11 -fopenmp
LFLAGS = -lgomp

SRCS = graph-minor-openMP.cpp mmio.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = mmio.h

TARGET = gm-mp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

cleanw:
	del /Q $(OBJS) $(TARGET)