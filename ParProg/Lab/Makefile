start:
	cmake -B build
	cmake --build build
	mpic++ mpi.cc -std=c++2a -o build/mpi

compile:
	cmake --build build
	mpic++ mpi.cc -std=c++2a -o build/mpi

mp:
	./build/openmp

mpi:
	mpiexec -n 4 ./build/mpi