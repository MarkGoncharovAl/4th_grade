#include "include/nucleus.hpp"
#include "gtest/gtest.h"

TEST (test , doubleDelete)
{
  Region<int> data;
  auto s1 = data.alloc(12);
  data.free(s1);
  data.free (s1);
}

int main(int argc, char * argv[]) {
  ::testing::InitGoogleTest (&argc , argv);
  return RUN_ALL_TESTS ();
}