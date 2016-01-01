CC = g++
CPPFLAGS = --std=c++14 -Wall -Wshadow -O2 -I. -DDEBUG
MAIN = fsim.cpp
TEST = testing.py
CPPFILES = $(filter-out $(TEST) $(MAIN), $(wildcard *.cpp))
OBJS = $(patsubst %.cpp, %.o, $(CPPFILES))

TESTPATH = ./tests
TESTCPP = $(wildcard $(TESTPATH)/*.cpp)
TESTEXE = $(patsubst $(TESTPATH)/%.cpp, $(TESTPATH)/exec/%, $(TESTCPP))

%.o: %.cpp %.h
	$(CC) $(CPPFLAGS) -c $<

.phony: all clean

all: $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) $(MAIN) -o fsim

run: all
	./fsim

tests/exec/%: $(OBJS) $(TESTPATH)/%.cpp
	$(CC) $(CPPFLAGS) $(OBJS) $(TESTPATH)/$*.cpp -o $@

testexec: $(TESTEXE)

test: testexec
	./testing.py $(arg)

s27: testexec
	./testing.py s27

clean:
	rm -f ./fsim
	rm -f $(OBJS)
	rm -f $(TESTEXE)
