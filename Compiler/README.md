# Compiler project

This project was created to practice my compiler knowledge

### Requirements

SFML must be downloaded.

### Build broject

Just an ordinary cmake building

```mkdir build && cd build && cmake ../ && cmake --build .```

### Explanation

![Math explanation](Heating.jpg)

Compiling C and C++ together with:

```bash 
clang main.c math/math.c -c -o main.o
clang++ graph.cpp -lsfml-graphics -lsfml-window -lsfml-system -c
clang++ main.o math.o graph.o -lsfml-graphics -lsfml-window -lsfml-system
```

It performs due to **link.h** and **link.hpp** same declaration.