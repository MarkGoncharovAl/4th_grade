SIZE ?= 400

all:
	@g++ main.cpp math.cpp -std=c++2a -DSIZE=$(SIZE) -pthread -ltbb -fopenmp
	@python3 matr.py $(SIZE) > tmp
	@./a.out
	@rm tmp a.out