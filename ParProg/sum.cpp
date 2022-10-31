#include "gtest/gtest.h"
#include <benchmark/benchmark.h>
#include <cmath>
#include <iostream>
#include <string>

int n = 0;

double one() {
  double sum = 1;
  for (int i = 2; i <= n; ++i) {
    sum += 1.0 / i;
  }
  return sum;
}

double many() {
  double sum = 1.0;
#pragma omp parallel for reduction(+ : sum) num_threads(8)
  for (int i = 2; i <= n; ++i) {
    sum += 1.0 / i;
  }
  return sum;
}

static void one_thread(benchmark::State &state) {
  for (auto _ : state)
    one();
}
BENCHMARK(one_thread);
static void many_thread(benchmark::State &state) {
  for (auto _ : state)
    many();
}
BENCHMARK(many_thread);

TEST(test, test1) { EXPECT_LE(std::fabs(one() - many()), 0.0000000001); }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Waits for argument - size!";
    return 1;
  }

  n = std::stoi(argv[1]);

  ::testing::InitGoogleTest(&argc, argv);
  int run_result = RUN_ALL_TESTS();

  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return run_result;
}