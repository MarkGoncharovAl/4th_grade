#include <iostream>
#include <omp.h>

int main(int argc, char *argv[]) {
  omp_set_max_active_levels(3);
  int thread0 = omp_get_thread_num();
  int size0 = omp_get_num_threads();
  
  #pragma omp parallel 
  {
    int thread1 = omp_get_thread_num();
    int size1 = omp_get_num_threads();
    #pragma omp parallel 
    {
      int thread2 = omp_get_thread_num();
      int size2 = omp_get_num_threads();
      #pragma omp parallel 
      {
        int thread3 = omp_get_thread_num();
        int size3 = omp_get_num_threads();
        #pragma omp crytical 
        {
          std::cout
          << " thread0: " << thread0 << " size0: " << size0 << std::endl
          << " thread1: " << thread1 << " size1: " << size1 << std::endl
          << " thread2: " << thread2 << " size2: " << size2 << std::endl
          << " thread3: " << thread3 << " size3: " << size3 << std::endl;
        }
      }
    }
  }
}