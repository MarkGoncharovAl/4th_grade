CC=clang
CXX=clang++

compile-llvm:
	${CC} -c math/my_math_ext.ll -o math.o
	${CC} -c main.c -o main.o
	${CC} -c graph.cpp -o graph.o
	${CXX} graph.o main.o math.o -lsfml-graphics -lsfml-window -lsfml-system -o main
	rm *.o