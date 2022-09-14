### Introduction : Sumary from 1 to 1/n

* ```g++ sum.cpp -fopenmp -lbenchmark -lpthread -lgtest```
* To run you have to pass size ```./a.out <n>```
* My result : on **n = 10000** size I win **5.73** times

### Extra : Copying directive

* ```g++ copyin.cpp -fopenmp```
* Just run and see the diference. Second case shows **Num : 0** , because every thread copied value from the master thread

### Extra : different_types

You can analyze using -DTYPE variable

### Extra : nested cycles
```
 thread0: 0 size0: 1
 thread1: 0 size1: 8
 thread2: 0 size2: 1
 thread3: 0 size3: 1
 thread0: 0 size0: 1
 thread1: 7 size1: 8
 thread2: 0 size2: 1
 thread3: 0 size3: 1
 thread0: 0 size0: 1
 thread1: 1 size1: 8
 thread2: 0 size2: 1
 thread3: 0 size3: 1
 thread0: 0 size0: 1
 thread1: 4 size1: 8
 thread2: 0 size2: 1
 thread3: 0 size3: 1
 thread0: 0 size0: 1
 thread1: 5 size1: 8
 thread2: 0 size2: 1
 thread3: 0 size3: 1
 thread0: 0 size0: 1
 thread1: 2 size1: 8
 thread2: 0 size2: 1
 thread3: 0 size3: 1
 thread0: 0 size0: 1
 thread1: 3 size1: 8
 thread2: 0 size2: 1
 thread3: 0 size3: 1
 thread0: 0 size0: 1
 thread1: 6 size1: 8
 thread2: 0 size2: 1
 thread3: 0 size3: 1
```
As we can see, we did create 3 cycles, but only the first one creates 8 threads.