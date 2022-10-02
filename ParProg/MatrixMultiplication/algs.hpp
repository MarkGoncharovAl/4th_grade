#include "matrix.hpp"

namespace dot
{
  template <typename T , int X , int Y , int Z>
  Matrix<T , X , Z> naive (const Matrix<T , X , Y>& lhs ,
                            const Matrix<T , Y , Z>& rhs)
  {
    Matrix<T , X , Z> out;
    for (int x = 0; x < X; ++x)
    {
      for (int z = 0; z < Z; ++z)
      {
        T sum = 0;
        for (int y = 0; y < Y; ++y)
          sum += lhs[x][y] * rhs[y][z];
        out[x][z] = sum;
      }
    }
    return out;
  }

  template <typename T , int X , int Y , int Z>
  Matrix<T , X , Z> outer_cycle (const Matrix<T , X , Y>& lhs ,
                                  const Matrix<T , Y , Z>& rhs)
  {
    Matrix<T , X , Z> out;

    for (int x = 0; x < X; ++x)
    {
      T* data = out[x];
      for (int y = 0; y < Y; ++y)
      {
        const T* r = rhs[y];
        const T l = lhs[x][y];
        for (int z = 0; z < Z; ++z)
          data[z] += l * r[z];
      }
    }
    return out;
  }

  namespace detail
  {
    template <typename T , int X , int Y> struct Matrixs
    {
      Matrix<T , X , Y> _11;
      Matrix<T , X , Y> _12;
      Matrix<T , X , Y> _21;
      Matrix<T , X , Y> _22;
    };

    constexpr int aprox_power_2 (int x)
    {
      int full_X = 1;
      while (full_X < x)
        full_X <<= 1;
      return full_X >> 1;
    }

    template <typename T , int X , int Y>
    Matrixs<T , aprox_power_2 (X) , aprox_power_2 (Y)>
      prepare_matrix (const Matrix<T , X , Y>& matr)
    {
      constexpr int X2 = aprox_power_2 (X);
      constexpr int Y2 = aprox_power_2 (Y);

      Matrixs<T , X2 , Y2> out;
      for (int i = 0; i < X2; ++i)
        for (int j = 0; j < Y2; ++j)
          out._11[i][j] = matr[i][j];
      for (int i = 0; i < X2; ++i)
        for (int j = Y2; j < Y; ++j)
          out._12[i][j - Y2] = matr[i][j];
      for (int i = X2; i < X; ++i)
        for (int j = 0; j < Y2; ++j)
          out._21[i - X2][j] = matr[i][j];
      for (int i = X2; i < X; ++i)
        for (int j = Y2; j < Y; ++j)
          out._22[i - X2][j - Y2] = matr[i][j];
      return out;
    }
  }

  template <typename T , int X , int Y , int Z , typename Dot>
  Matrix<T , X , Z> shtrass (const Matrix<T , X , Y>& lhs ,
                              const Matrix<T , Y , Z>& rhs ,
                              Dot f = naive)
  {
    // Prepare matrix
    auto A = detail::prepare_matrix (lhs);
    auto B = detail::prepare_matrix (rhs);
    auto D = f (A._11 + A._22 , B._11 + B._22);
    auto D1 = f (A._12 - A._22 , B._21 + B._22);
    auto D2 = f (A._21 - A._11 , B._11 + B._12);
    auto H1 = f (A._11 + A._12 , B._22);
    auto H2 = f (A._21 + A._22 , B._11);
    auto V1 = f (A._22 , B._21 - B._11);
    auto V2 = f (A._11 , B._12 + B._22);


  }
}





