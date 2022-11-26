#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <random>
#include <fstream>
#include <execution>
#include <chrono>

#ifndef SIZE
#define SIZE 400
#endif

constexpr double step = 1.0 / SIZE;
constexpr size_t size = SIZE;

double f (double data);
double rightSizeIn (double lhs , double mid , double rhs);
double rightSize (double lhs , double mid , double rhs);

using vector = std::vector<double>;
using matrix = std::vector<std::vector<double>>;

matrix initA ();
matrix initA_1 ();
vector initY ();
void dump (const vector& data , std::string comment = "");
void dump (const matrix& data , std::string comment = "");
double dumpMer (const vector& data , std::string comment = "");

namespace Seq
{
  vector calcF (const vector& vec);
  vector Minus (const vector& lhs , const vector& rhs);
  vector Plus (const vector& lhs , const vector& rhs);
  vector Mult (const matrix& lhs , const vector& rhs);
  vector Div (const vector& lhs , double rhs);
  double dumpMer (const vector& data);
}

namespace Par
{
  vector calcF (const vector& vec);
  vector Minus (const vector& lhs , const vector& rhs);
  vector Plus (const vector& lhs , const vector& rhs);
  vector Mult (const matrix& lhs , const vector& rhs);
  vector Div (const vector& lhs , double rhs);
  double dumpMer (const vector& data);
}