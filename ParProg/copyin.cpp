#include <iostream>
#include <omp.h>

int threadPrivate = 7;
#pragma omp threadprivate(threadPrivate)

void without() {
  threadPrivate = 7;
#pragma omp parallel
{
  threadPrivate = omp_get_thread_num();
}
#pragma omp parallel
{
    std::cout << "Num: " << threadPrivate << " thread: " << omp_get_thread_num() << "\n";
}  
}

void with() {
  threadPrivate = 7;
#pragma omp parallel
{
  threadPrivate = omp_get_thread_num();
}
#pragma omp parallel copyin(threadPrivate)
{
    std::cout << "Num: " << threadPrivate << " thread: " << omp_get_thread_num() << "\n";
}  
}

int main(){
  omp_set_num_threads(4); // for convenient view result

  std::cout << "Without copyin method\n\n";
  without();

  std::cout << "Using copyin method\n\n";
  with();
}