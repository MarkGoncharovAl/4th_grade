#include <omp.h>
#include <iostream>

int main(int argc, char *argv[]) {
  int n = 8;
  int final = 0;

  #pragma omp parallel for schedule(TYPE)
  for (int i = 0; i < n; ++i)
  {
      #pragma omp crytical 
      {
        std::cout<< "Num: " << i << " thread: " << omp_get_thread_num() << "\n";
      }
    }
}