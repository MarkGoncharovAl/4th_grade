#include "include/nucleus.h"
#include "gtest/gtest.h"

TEST (test , doubleDelete)
{
  auto s1 = Alloc();
  EXPECT_FALSE(s1 == NULL);
  EXPECT_EQ(Free(&s1), ErrCode::Non);
  EXPECT_EQ(Free(&s1), ErrCode::FreeZero);
}

TEST(test, leak)
{
  auto s1 = Alloc();
  EXPECT_FALSE(s1 == NULL);
}

TEST(test, free_null)
{
  EXPECT_EQ(Free(nullptr), ErrCode::FreeZero);
}

TEST(test, read_null)
{
  Read (nullptr);
  EXPECT_EQ(Check(), ErrCode::ReadZero);
}

TEST(test, write_null)
{
  EXPECT_EQ(Write(nullptr, 24), ErrCode::WriteZero);
}

TEST (test, read_freed)
{
  auto s1 = Alloc();
  EXPECT_EQ(Free(&s1), ErrCode::Non);
  Read (s1);
  EXPECT_EQ(Check(), ErrCode::ReadZero);
}

TEST (test, write_freed)
{
  auto s1 = Alloc();
  EXPECT_EQ(Free(&s1), ErrCode::Non);
  EXPECT_EQ(Write(s1, 24), ErrCode::WriteZero);
}

int main(int argc, char * argv[]) {
  ::testing::InitGoogleTest (&argc , argv);
  return RUN_ALL_TESTS ();
}