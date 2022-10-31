#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <random>
#include <vector>

// To print more data use define "debug"

namespace implement {
inline void act(int stat, size_t line, const char file[]) {
  if (stat == MPI_SUCCESS) {
#ifdef debug
    printf("OK! File: %s, line: %d\n", file, line);
#endif
    return;
  }
  printf("ERROR!\nStatus: %d in file: %s, line: %ld\n\n", stat, file, line);
  std::exit(1);
}

inline void check(int stat, const char message[], size_t line,
                  const char file[]) {
  if (stat) {
#ifdef debug
    printf("OK! File: %s, line: %d\n", file, line);
#endif
    return;
  }
  printf("ERROR!\n%s.\nFails in file: %s, line: %ld\n\n", message, file, line);
  std::exit(1);
}

inline void quit(size_t line, const char file[]) {
  implement::act(MPI_Finalize(), line, file);
  std::exit(0);
}
} // namespace implement

#define act(stat) implement::act(stat, __LINE__, __FILE__)
#define check(stat, message) implement::check(stat, message, __LINE__, __FILE__)
#define quit() implement::quit(__LINE__, __FILE__)

namespace mpi {
constexpr int rank_main = 0;
std::pair</*size*/ int, /*rank*/ int> init() {
  int size = 0, rank = 0;
  act(MPI_Init(NULL, NULL));
  act(MPI_Comm_size(MPI_COMM_WORLD, &size));
  act(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
  return std::make_pair(size, rank);
}
} // namespace mpi

constexpr int max(int lhs, int rhs) { return lhs < rhs ? rhs : lhs; }

class Rand {
private:
  std::random_device dev;
  std::mt19937 rng;
  std::uniform_int_distribution<std::mt19937::result_type> dist;

public:
  Rand(unsigned long left = -10000, unsigned long right = 10000)
      : dev{}, rng{dev()}, dist{left, right} {}
  unsigned long get() { return dist(rng); }
  template <typename T = int> std::vector<T> getVector(int size) {
    std::vector<T> data(size);
    for (auto &elem : data) {
      elem = static_cast<T>(get());
    }
    return data;
  }
};