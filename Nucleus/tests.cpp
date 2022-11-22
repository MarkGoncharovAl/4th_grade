#include "include/nucleus.h"
#include "gtest/gtest.h"

TEST (test , doubleDelete)
{
  auto s1 = Alloc();
  EXPECT_FALSE(s1 == NULL);
  EXPECT_EQ((int)Free(s1), 0);
  // EXPECT_EQ((int)Free(s1), 2);
}

TEST(test, leak)
{
  auto s1 = Alloc();
  EXPECT_FALSE(s1 == NULL);
}

TEST(test, free_null)
{
  EXPECT_EQ((int)Free(nullptr), 2);
}

TEST(test, read_null)
{
  EXPECT_EQ(Read(nullptr), 3);
}

TEST(test, write_null)
{
  EXPECT_EQ((int)Write(nullptr, 24), 4);
}

TEST (test, read_freed)
{
  auto s1 = Alloc();
  //Check();
  EXPECT_EQ((int)Free(s1), 0);
  EXPECT_EQ(Read(s1), 3);
}

TEST (test, write_freed)
{
  auto s1 = Alloc();
  //Check();
  EXPECT_EQ((int)Free(s1), 0);
  EXPECT_EQ((int)Write(s1, 24), 4);
}

int main(int argc, char * argv[]) {
  ::testing::InitGoogleTest (&argc , argv);
  return RUN_ALL_TESTS ();
}