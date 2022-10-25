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

#### Метод оптимизации

Здесь вектор расстояния **D = (-1, 1)**, вектор направления 
**d = (>, <)**, имеется антизависимость (значения сначала используются, 
а потом определяются).

По теории, распараллелить можно по внутреннему циклу с барьерной синхронизацией 
после одного внут цикла или предварительного копирования нужных данных на исполнителях

Также в данном примере нельзя поменять местами внешний и внутренний циклы!

#### Win **3.53** times

| 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|
| 1.74 |  2.37 | 2.98  | 2.69  |  2.87  |  3.02  |  3.53  |

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


#### Метод оптимизации

Здесь вектор расстояния **D = (1, 3)**, вектор направления 
**d = (<, <)**, имеется истинная зависимость (значения сначала вычисляются, 
а потом используются).

По теории, распараллелить можно ТОЛЬКО по внутреннему циклу с барьерной синхронизацией 
после одного внут цикла.

Однако в истинной зависимости можно поменять местами циклы,
что тоже может дать прирост. НО

1. Если мы меняем циклы, то ломаем локальность кэшей
2. Если мы хотим хорошую работу с кэшем, то надо транспонировать матрицу
3. Затраты на транспонирование матрицы сопоставимы со сложностью вычисления

Поэтому был распараллелен внутренний цикл с барьерной оптимизацией.


#### Win **1.21** times.

## Conclusion

We parallel algorithms according to theory and get some 
appropriate acceleration.