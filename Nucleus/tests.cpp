#include "include/nucleus.h"
#include "gtest/gtest.h"

TEST (test , doubleDelete)
{
  auto s1 = Alloc();
  //Check();
  Free(s1);
  Free (s1);
}

int main(int argc, char * argv[]) {
  ::testing::InitGoogleTest (&argc , argv);
  return RUN_ALL_TESTS ();
}