
all: tp1

tp1:
	g++ -o tp1-problema1 tp1-problema1.cpp -std=c++11
	g++ -o tp1-problema3 tp1-problema3.cpp -std=c++11



clean:
	rm -f *.o
	@echo Old binaries deleted...
	@echo Creating new binaries ...
