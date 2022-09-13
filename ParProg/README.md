### Introduction : Sumary from 1 to 1/n

* ```g++ sum.cpp -fopenmp -lbenchmark -lpthread -lgtest```
* To run you have to pass size ```./a.out <n>```
* My result : on **n = 10000** size I win **5.73** times

### Extra : Copying directive

* ```g++ copyin.cpp -fopenmp```
* Just run and see the diference. Second case shows **Num : 0** , because every thread copied value from the master thread