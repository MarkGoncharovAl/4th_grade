#include "common.hpp"

double f (double data)
{
  return std::pow (2.71 , data);
}
double rightSizeIn (double lhs , double mid , double rhs)
{
  return mid + (lhs - 2 * mid + rhs) / 12.0;
}
double rightSize (double lhs , double mid , double rhs)
{
  return rightSizeIn (f (lhs) , f (mid) , f (rhs));
}

using vector = std::vector<double>;
using matrix = std::vector<std::vector<double>>;

matrix initA ()
{
  constexpr double mid = -2.0 / (step * step);
  constexpr double side = 1.0 / (step * step);
  matrix A (size);
  for (auto& vec : A)
  {
    vec.resize (size);
    for (auto& elem : vec)
      elem = 0.0;
  }

  A[0][0] = mid;
  A[0][1] = side;
  A[size - 1][size - 1] = mid;
  A[size - 1][size - 2] = side;
  for (int i = 1; i < size - 1; ++i)
  {
    A[i][i - 1] = side;
    A[i][i] = mid;
    A[i][i + 1] = side;
  }
  return A;
}
matrix initA_1 ()
{
  std::ifstream file ("tmp");
  double num;
  vector storage (size * size);
  auto iter = storage.begin ();
  while (file >> num)
  {
    if (iter == storage.end ())
      break;
    *iter = num;
    ++iter;
  }
  iter = storage.begin ();
  matrix out (size);
  for (int i = 0; i < size; ++i)
  {
    out[i].resize (size);
    for (int j = 0; j < size; ++j)
    {
      out[i][j] = *iter;
      ++iter;
    }
  }
  return out;
}

vector initY ()
{
  std::random_device dev;
  std::mt19937 rng (dev ());
  std::uniform_int_distribution<std::mt19937::result_type> dist6 (1 , 20);
  vector Y (size);
  double small_step = (b - 1) / (size + 1);
  for (int i = 0; i < size; ++i) {
    Y[i] = 0.9999 + small_step * (i + 1) + (double)dist6 (rng) / 100000.0;
  }
  return Y;
}

void dump (const vector& data, std::string comment)
{
  std::cout << comment << "\n";
  std::copy (data.cbegin () , data.cend () , std::ostream_iterator<double> (std::cout , " "));
  std::cout << "\n";
}
void dump (const matrix& data , std::string comment)
{
  std::cout << comment << "\n";
  for (auto const& vec : data)
  {
    std::copy (vec.cbegin () , vec.cend () , std::ostream_iterator<double> (std::cout , " "));
    std::cout << "\n";
  }
}
double dumpMer (const vector& data , std::string comment)
{
  std::cout << comment << " Mer : ";
  double sum = std::transform_reduce (data.cbegin () , data.cend () , 0.0 , std::plus {} , [](const double elem) { return elem * elem; });
  std::cout << sum << "\n";
  return sum;
}

namespace Seq
{
  vector calcF (const vector& vec)
  {
    vector F (size);
    F[0] = rightSize (1.0 , vec[0] , vec[1]) - 1.0 / (step * step);
    F[size - 1] = rightSize (vec[size - 2] , vec[size - 1] , 1.0) - b / (step * step);
    for (int i = 1; i < size - 1; ++i)
    {
      F[i] = rightSize (vec[i - 1] , vec[i] , vec[i + 1]);
    }
    return F;
  }

  vector Minus (const vector& lhs , const vector& rhs)
  {
    vector out (size);
    for (int i = 0; i < size; ++i)
      out[i] = lhs[i] - rhs[i];
    return out;
  }
  vector Plus (const vector& lhs , const vector& rhs)
  {
    vector out (size);
    for (int i = 0; i < size; ++i)
      out[i] = lhs[i] + rhs[i];
    return out;
  }

  vector Mult (const matrix& lhs , const vector& rhs)
  {
    vector out (size);
    for (int i = 0; i < size; ++i)
    {
      out[i] = 0;
      for (int j = 0; j < size; ++j)
        out[i] += lhs[i][j] * rhs[j];
    }
    return out;
  }

  vector Div (const vector& lhs , double rhs)
  {
    vector out (size);
    for (int i = 0; i < size; ++i)
      out[i] = lhs[i] / rhs;
    return out;
  }
  double dumpMer (const vector& data)
  {
    return std::transform_reduce (std::execution::seq , data.cbegin () , data.cend () , 0.0 , std::plus {} , [](const double elem) { return elem * elem; });
  }
}


// -------------------------------------------------------------------- //
// -------------------------------------------------------------------- //


namespace Par
{
  vector calcF (const vector& vec)
  {
    vector F (size);
#pragma parallel for num_thread(8)
    for (int k = 0; k < 8; ++k)
    {
      int offset = k * size / 8;
      for (int i = offset; i < offset + size / 8; ++i)
        F[i] = rightSize (vec[i - 1] , vec[i] , vec[i + 1]);
    }
    F[0] = rightSize (1.0 , vec[0] , vec[1]) - 1.0 / (step * step);
    F[size - 1] = rightSize (vec[size - 2] , vec[size - 1] , 1.0) - b / (step * step);
    return F;
  }

  vector Minus (const vector& lhs , const vector& rhs)
  {
    vector out (size);
#pragma parallel for num_thread(8)
    for (int k = 0; k < 8; ++k)
    {
      int offset = k * size / 8;
      for (int i = offset; i < offset + size / 8; ++i)
        out[i] = lhs[i] - rhs[i];
    }
    return out;
  }
  vector Plus (const vector& lhs , const vector& rhs)
  {
    vector out (size);
#pragma parallel for num_thread(8)
    for (int k = 0; k < 8; ++k)
    {
      int offset = k * size / 8;
      for (int i = offset; i < offset + size / 8; ++i)
        out[i] = lhs[i] + rhs[i];
    }
    return out;
  }

  vector Mult (const matrix& lhs , const vector& rhs)
  {
    vector out (size);
#pragma parallel for num_thread(8)
    for (int k = 0; k < 8; ++k)
    {
      int offset = k * size / 8;
      for (int i = offset; i < offset + size / 8; ++i)
      {
        out[i] = 0;
        for (int j = 0; j < size; ++j)
          out[i] += lhs[i][j] * rhs[j];
      }
    }
    return out;
  }

  vector Div (const vector& lhs , double rhs)
  {
    vector out (size);
#pragma parallel for num_thread(8)
    for (int k = 0; k < 8; ++k)
    {
      int offset = k * size / 8;
      for (int i = offset; i < offset + size / 8; ++i)
        out[i] = lhs[i] / rhs;
    }
    return out;
  }
  double dumpMer (const vector& data)
  {
    return std::transform_reduce (data.cbegin () , data.cend () , 0.0 , std::plus {} , [](const double elem) { return elem * elem; });
  }
}