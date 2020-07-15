CPLUSPLUS = g++ -std=c++11 -O2 -Wall 
DEL = rm -f

VERIFIER = ../public/verifier

all:
	$(CPLUSPLUS) -pthread src/b04705001.cpp -o solver
mac:
	$(CPLUSPLUS) src/b04705001.cpp -o solver
linux: 
	$(CPLUSPLUS) -pthread src/b04705001.cpp -o solver
verify:
	$(VERIFIER) -i testdata/small.in -o testdata/small.out
	$(VERIFIER) -i testdata/large0.in -o testdata/large0.out
	$(VERIFIER) -i testdata/large1.in -o testdata/large1.out
	$(VERIFIER) -i testdata/large2.in -o testdata/large2.out

clean:
	$(DEL) solver
