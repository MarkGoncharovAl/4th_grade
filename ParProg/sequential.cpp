#include <iostream>
#include <string>
#include <omp.h>

int main (int argc , char* argv [])
{
  if (argc != 2)
  {
    std::cerr << "Wait for initValue";
    return 1;
  }

  int initVal = std::stoi (argv[1]);
  int NumThreads = omp_get_max_threads ();

#pragma omp parallel for ordered
  for (int i = 0; i < NumThreads; ++i)
  {
    auto CurThread = omp_get_thread_num ();
#pragma omp critical
    std::cout << "Thread\t" << CurThread << "\tIteration:\t" << i
      << "\tCurValue\t" << initVal++ << std::endl;
  }
}