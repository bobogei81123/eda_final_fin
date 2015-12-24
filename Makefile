CC = g++
CPPFLAGS = --std=c++14 -Wall -Wshadow -O2
CPPFILES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(CPPFILES))

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $<

all: $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o fsim

