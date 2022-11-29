#include "gtest/gtest.h"
#include "include/nucleusN.hpp"


TEST (test , doubleDelete)
{
  struct table tb = init_table(sizeof(int));
  struct index in = createNew(&tb);
  EXPECT_EQ(Free(&tb, &in), ErrCode::Non);
  EXPECT_EQ(Free(&tb, &in), ErrCode::FreeZero);
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Non);
  #ifdef DEBUG
  EXPECT_EQ(check_bounds(&tb), ErrCode::Non);
  #endif
  destroy(&tb);
}

#ifdef DEBUG
TEST (test, write_out_of_bounds)
{
  struct table tb = init_table(sizeof(char));
  struct index in = createNew(&tb);
  EXPECT_EQ(Check(), ErrCode::Non);
  void* pt = get(&tb, &in);
  EXPECT_EQ(Check(), ErrCode::Non);
  EXPECT_FALSE(pt == nullptr);
  int* pt2 = (int*)pt;
  *(pt2) = (int)MAXINT;
  EXPECT_EQ(check_bounds(&tb), ErrCode::OutOfBounds);
  destroy(&tb);
}
#endif

TEST(test, free_null)
{
  struct table tb = init_table(sizeof(int));
  struct index in1 = {1, 0, 0};
  struct index in2 = {0, 1, 0};
  struct index in3 = {0, 0, 1};

  EXPECT_EQ(Free(&tb, &in1), ErrCode::FreeZero);
  EXPECT_EQ(Free(&tb, &in2), ErrCode::FreeZero);
  EXPECT_EQ(Free(&tb, &in3), ErrCode::FreeZero);

  struct index in4 = createNew(&tb);
  EXPECT_EQ(Free(&tb, &in3), ErrCode::WrongGen);
  EXPECT_EQ(Free(&tb, &in4), ErrCode::Non);
  #ifdef DEBUG
  EXPECT_EQ(check_bounds(&tb), ErrCode::Non);
  #endif
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Non);
  destroy(&tb);
}

TEST(test, get_null)
{
  struct table tb = init_table(sizeof(int));
  struct index in1 = {1, 0, 0};
  struct index in2 = {0, 1, 0};
  struct index in3 = {0, 0, 1};
  void * pt1 = get(&tb,&in1);
  EXPECT_EQ(Check(), ErrCode::GetZero);
  EXPECT_EQ(pt1, nullptr);
  void * pt2 = get(&tb,&in2);
  EXPECT_EQ(Check(), ErrCode::GetZero);
  EXPECT_EQ(pt2, nullptr);
  void * pt3 = get(&tb,&in3);
  EXPECT_EQ(Check(), ErrCode::GetZero);
  EXPECT_EQ(pt3, nullptr);
  #ifdef DEBUG
  EXPECT_EQ(check_bounds(&tb), ErrCode::Non);
  #endif
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Non);
  destroy(&tb);
}

TEST (test, get_freed)
{
  struct table tb = init_table(sizeof(int));
  struct index in = createNew(&tb);
  EXPECT_EQ(Free(&tb, &in), ErrCode::Non);
  void* pt = get(&tb, &in);
  EXPECT_EQ(Check(), ErrCode::GetZero);
  EXPECT_EQ(pt, nullptr);
  #ifdef DEBUG
  EXPECT_EQ(check_bounds(&tb), ErrCode::Non);
  #endif
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Non);
  destroy(&tb);
}

TEST (test, get)
{
  struct table tb = init_table(sizeof(int));
  struct index in = createNew(&tb);
  void* pt = get(&tb, &in);
  EXPECT_EQ(Check(), ErrCode::Non);
  EXPECT_FALSE(pt == nullptr);
  #ifdef DEBUG
  EXPECT_EQ(check_bounds(&tb), ErrCode::Non);
  #endif
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Leaks);
  destroy(&tb);
}

TEST (test, write_read)
{
  struct table tb = init_table(sizeof(int));
  struct index in = createNew(&tb);
  int* pt = (int*)get(&tb, &in);
  EXPECT_EQ(Check(), ErrCode::Non);
  EXPECT_FALSE(pt == nullptr);
  *(pt) = 2022;
  int i = *(pt);
  EXPECT_EQ(i, 2022);
  #ifdef DEBUG
  EXPECT_EQ(check_bounds(&tb), ErrCode::Non);
  #endif
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Leaks);
  destroy(&tb);
}

TEST (test, many_writes)
{
  struct table tb = init_table(sizeof(int));
  struct index in1 = createNew(&tb);
  struct index in2 = createNew(&tb);
  struct index in3 = createNew(&tb);
  struct index in4 = createNew(&tb);

  int* pt1 = (int*)get(&tb, &in1);
  EXPECT_EQ(Check(), ErrCode::Non);
  int* pt2 = (int*)get(&tb, &in2);
  EXPECT_EQ(Check(), ErrCode::Non);
  int* pt3 = (int*)get(&tb, &in3);
  EXPECT_EQ(Check(), ErrCode::Non);
  int* pt4 = (int*)get(&tb, &in4);
  EXPECT_EQ(Check(), ErrCode::Non);


  *(pt1) = 2021;
  *(pt2) = 2026;
  *(pt3) = 2031;
  *(pt4) = 2048;
  #ifdef DEBUG
  EXPECT_EQ(check_bounds(&tb), ErrCode::Non);
  #endif
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Leaks);
  destroy(&tb);
}

TEST (test, alloc)
{
  struct table tb = init_table(sizeof(int));
  struct index in = createNew(&tb);
  EXPECT_EQ(in.code1, 0);
  EXPECT_EQ(in.code2, 0);
  EXPECT_EQ(in.code3, 0);
  #ifdef DEBUG
  EXPECT_EQ(check_bounds(&tb), ErrCode::Non);
  #endif
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Leaks);
  destroy(&tb);
}

TEST (test, free)
{
  struct table tb = init_table(sizeof(int));
  struct index in = createNew(&tb);
  EXPECT_EQ(Free(&tb, &in), ErrCode::Non);
  #ifdef DEBUG
  EXPECT_EQ(check_bounds(&tb), ErrCode::Non);
  #endif
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Non);
  destroy(&tb);
}

TEST(test, leak)
{
  struct table tb = init_table(sizeof(int));
  createNew(&tb);
  EXPECT_EQ(Check(), ErrCode::Non);
  check_leaks(&tb);
  EXPECT_EQ(Check(), ErrCode::Leaks);
}

int main(int argc, char * argv[]) {
  ::testing::InitGoogleTest (&argc , argv);
  return RUN_ALL_TESTS ();
}