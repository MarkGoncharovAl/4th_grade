#include "include/nucleus.h"
#include "gtest/gtest.h"
#include "include/nucleusNew.hpp"


TEST (test , doubleDelete)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  EXPECT_EQ(A.Free(pt.first), ErrCode::Non);
  EXPECT_EQ(A.Free(pt.first), ErrCode::FreeZero);
}

//! TODO
// TEST(test, leak)
// {
//   int* pt_out = nullptr;
//   for (int i = 0; i < 1; i++) {
//     managerMemory_t <int> A;
//     auto pt = A.createNew();
//     pt_out = pt.second;
//   }
// }


TEST(test, free_null)
{
  managerMemory_t <int> A;
  EXPECT_EQ(A.Free(20), ErrCode::FreeZero);
}

TEST(test, get_null)
{
  managerMemory_t <int> A;
  auto pt = A.get(20);
  EXPECT_EQ(Check(), ErrCode::GetZero);
  EXPECT_EQ(pt, nullptr);
}


TEST (test, get_freed)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  EXPECT_EQ(A.Free(pt.first), ErrCode::Non);
  auto pt2 = A.get(pt.first);
  EXPECT_EQ(Check(), ErrCode::GetZero);
  EXPECT_EQ(pt2, nullptr);
}

TEST (test, get)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  auto pt2 = A.get(pt.first);
  EXPECT_EQ(Check(), ErrCode::Non);
  EXPECT_FALSE(pt2 == nullptr);
}

TEST (test, write_read)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  auto pt2 = A.get(pt.first);
  EXPECT_EQ(Check(), ErrCode::Non);
  EXPECT_FALSE(pt2 == nullptr);
  *(pt2) = 2022;
  auto i = *(pt2);
  EXPECT_EQ(i, 2022);
}

TEST (test, alloc)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  EXPECT_FALSE(pt == std::make_pair(0, (int*)nullptr));
}

TEST (test, free)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  EXPECT_FALSE(pt == std::make_pair(0, (int*)nullptr));
  EXPECT_EQ(A.Free(pt.first), ErrCode::Non);
}

int main(int argc, char * argv[]) {
  ::testing::InitGoogleTest (&argc , argv);
  return RUN_ALL_TESTS ();
}