# Lab work for parallel cycles

[Description](./lab.pdf)

## OpenMP

#### First cycle
```cpp
auto size = ISIZE / 8;
#pragma omp parallel for
for (int k = 0; k < 8; ++k)
  for (int i = k * size; i < (k + 1) * size; ++i)
    for (int j = 0; j < JSIZE; ++j)
```

#### Second cycle
```cpp
for (int i = 0; i < ISIZE - 1; ++i)
#pragma omp parallel for
    for (int j = 1; j < JSIZE; ++j)
```

#### Interface
We create random array, test output of sequential and parallel algorithms
```bash
[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (197 ms total)
[  PASSED  ] 1 test.
```
and compare the speed
```bash
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
sequential2A_bench   30159496 ns     30151981 ns           22
parallel2A_bench     12075646 ns     12037786 ns           57
```

#### Win **2.95** times

## MPI

We have rank0 for sending and receiving info from threads.

#### First cycle
```cpp
auto curSize = ISIZE / (size - 1);
for (int i = (rank - 1) * curSize; i < rank * curSize; ++i)
  for (int j = 0; j < JSIZE; ++j)
```

#### Second cycle
```cpp
int cur2Size = (JSIZE - 3) / (size - 1);
for (int i = 1; i < ISIZE; ++i) {
  auto start_elem = 3 + (rank - 1) * cur2Size;
  for (int j = start_elem; j < start_elem + cur2Size; ++j)
```

#### Interface
The same method
```bash
Algorithm works fine! #check for equal final matrix
Time sequential: 0.0977309
Time parallel: 0.0871089
```

#### Win **1.12** times.

## Conclusion

We parallel algorithms according to theory and get some 
appropriate acceleration.