SIZE ?= 400
B ?= 1

all:
	@g++ main.cpp math.cpp -std=c++2a -DSIZE=$(SIZE) -DB=$(B) -pthread -ltbb -fopenmp
	@python3 matr.py $(SIZE) > tmp
	@./a.out
	@rm tmp a.out