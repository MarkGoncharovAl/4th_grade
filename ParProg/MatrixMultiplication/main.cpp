#include "algs.hpp"
#include <gtest/gtest.h>
#include <iostream>

int main (int argc , char* argv [])
{
  ::testing::InitGoogleTest (&argc , argv);
  return RUN_ALL_TESTS ();
}

TEST (Matrix , Equal)
{
  Matrix<int , 3 , 3> matr = { 1, 2, 3, 1, 2, 3, 1, 2, 3 };
  Matrix<int , 3 , 3> matr2 = { 1, 2, 3, 1, 2, 3, 1, 2, 3 };
  EXPECT_TRUE (matr == matr2);
}
TEST (Matrix , Sum)
{
  Matrix<int , 3 , 3> matr = { 1, 2, 3, 1, 2, 3, 1, 2, 3 };
  Matrix<int , 3 , 3> matr2 = { 1, 3, 3, 2, -1, 3, 1, 2, 3 };
  Matrix<int , 3 , 3> matr3 = { 2, 5, 6, 3, 1, 6, 2, 4, 6 };
  EXPECT_TRUE (matr + matr2 == matr3);
}
TEST (Mult , Naive1)
{
  Matrix<int , 3 , 3> matr = { 1, 0, 0, 0, 2, 0, 0, 0, 3 };
  Matrix<int , 3 , 3> matr2 = { 1, 2, 3, 1, 2, 3, 1, 2, 3 };
  Matrix<int , 3 , 3> matr3 = { 1, 2, 3, 2, 4, 6, 3, 6, 9 };
  EXPECT_TRUE (dot::naive (matr , matr2) == matr3);
  EXPECT_TRUE (dot::outer_cycle (matr , matr2) == matr3);
  dot::shtrass (matr , matr2);
}
TEST (Mult , Naive2)
{
  Matrix<int , 3 , 3> matr = { 1, 0, 1, 0, 1, 1, 1, 1, 0 };
  Matrix<int , 3 , 3> matr2 = { 1, 2, 3, 1, 2, 3, 1, 2, 3 };
  Matrix<int , 3 , 3> matr3 = { 2, 4, 6, 2, 4, 6, 2, 4, 6 };
  EXPECT_TRUE (dot::naive (matr , matr2) == matr3);
  EXPECT_TRUE (dot::outer_cycle (matr , matr2) == matr3);
}
TEST (Mult , Naive3)
{
  Matrix<int , 2 , 3> matr = { 1, 0, 1, 0, 1, 1 };
  Matrix<int , 3 , 2> matr2 = { 1, 2, 1, 2, 1, 2 };
  Matrix<int , 2 , 2> matr3 = { 2, 4, 2, 4 };
  EXPECT_TRUE (dot::naive (matr , matr2) == matr3);
  EXPECT_TRUE (dot::outer_cycle (matr , matr2) == matr3);
}
TEST (Mult , Naive4)
{
  Matrix<int , 2 , 4> matr = { 1, 2, 1, 5, 3, 1, 3, 3 };
  Matrix<int , 4 , 3> matr2 = { 2, 1, 2, 1, 1, 4, 1, 4, 2, 3, 3, 1 };
  Matrix<int , 2 , 3> matr3 = { 20, 22, 17, 19, 25, 19 };
  EXPECT_TRUE (dot::naive (matr , matr2) == matr3);
  EXPECT_TRUE (dot::outer_cycle (matr , matr2) == matr3);
}