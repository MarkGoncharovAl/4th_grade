#include "include/nucleus.h"
#include "gtest/gtest.h"
#include "include/nucleusNew.hpp"


TEST (test , doubleDelete)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  EXPECT_EQ(A.Free(pt.first), ErrCode::Non);
  EXPECT_EQ(A.Free(pt.first), ErrCode::FreeZero);
  #ifdef DEBUG
  EXPECT_EQ(A.check_bounds(), ErrCode::Non);
  #endif
}

#ifdef DEBUG
TEST (test, write_out_of_bounds)
{
  managerMemory_t <char> A;
  std::pair<int, char*> pt = A.createNew();
  int* pt2 = (int*)A.get(pt.first);
  EXPECT_EQ(Check(), ErrCode::Non);
  EXPECT_FALSE(pt2 == nullptr);
  EXPECT_EQ((char*)pt2, pt.second);
  *(pt2) = 2022;
  EXPECT_EQ(A.check_bounds(), ErrCode::OutOfBounds);
}
#endif

TEST(test, free_null)
{
  managerMemory_t <int> A;
  EXPECT_EQ(A.Free(20), ErrCode::FreeZero);
  #ifdef DEBUG
  EXPECT_EQ(A.check_bounds(), ErrCode::Non);
  #endif
}

TEST(test, get_null)
{
  managerMemory_t <int> A;
  auto pt = A.get(20);
  EXPECT_EQ(Check(), ErrCode::GetZero);
  EXPECT_EQ(pt, nullptr);
  #ifdef DEBUG
  EXPECT_EQ(A.check_bounds(), ErrCode::Non);
  #endif
}

TEST (test, get_freed)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  EXPECT_EQ(A.Free(pt.first), ErrCode::Non);
  auto pt2 = A.get(pt.first);
  EXPECT_EQ(Check(), ErrCode::GetZero);
  EXPECT_EQ(pt2, nullptr);
  #ifdef DEBUG
  EXPECT_EQ(A.check_bounds(), ErrCode::Non);
  #endif
}

TEST (test, get)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  auto pt2 = A.get(pt.first);
  EXPECT_EQ(Check(), ErrCode::Non);
  EXPECT_FALSE(pt2 == nullptr);
  #ifdef DEBUG
  EXPECT_EQ(A.check_bounds(), ErrCode::Non);
  #endif
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
  #ifdef DEBUG
  EXPECT_EQ(A.check_bounds(), ErrCode::Non);
  #endif
}

TEST (test, many_writes)
{
  managerMemory_t <int> A;
  auto pt1 = A.createNew();
  auto pt2 = A.createNew();
  auto pt3 = A.createNew();
  auto pt4 = A.createNew();
  *(pt1.second) = 2021;
  *(pt2.second) = 2026;
  *(pt3.second) = 2031;
  *(pt4.second) = 2048;
  #ifdef DEBUG
  EXPECT_EQ(A.check_bounds(), ErrCode::Non);
  #endif
}

TEST (test, alloc)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  EXPECT_FALSE(pt == std::make_pair(0, (int*)nullptr));
  #ifdef DEBUG
  EXPECT_EQ(A.check_bounds(), ErrCode::Non);
  #endif
}

TEST (test, free)
{
  managerMemory_t <int> A;
  auto pt = A.createNew();
  EXPECT_FALSE(pt == std::make_pair(0, (int*)nullptr));
  EXPECT_EQ(A.Free(pt.first), ErrCode::Non);
  #ifdef DEBUG
  EXPECT_EQ(A.check_bounds(), ErrCode::Non);
  #endif
}

// ! TODO
TEST(test, leak)
{
  int* pt_out = nullptr;
  for (int i = 0; i < 1; i++) {
    managerMemory_t <int> A;
    auto pt = A.createNew();
    pt_out = pt.second;
  }
  EXPECT_EQ(Check(), ErrCode::Non);
}

int main(int argc, char * argv[]) {
  ::testing::InitGoogleTest (&argc , argv);
  return RUN_ALL_TESTS ();
}