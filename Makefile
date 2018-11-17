COMPILER=g++
CXXSOURCES=one_process.cpp
CXXOBJECTS=one_process.o

all: 
	g++ -o correct one_process.cpp
	./correct 
	mpicc -o run b.c -lm
	mpirun --hostfile hostfile -np 5 run 
	g++ -o check check.cpp 
	./check

run_c:
	./one_process.o

run:
	mpicc -o run b.c -lm

run1:
	mpirun --hostfile hostfile -np 5 run

test:
	./check.o

clean:
	rm -rf *.o