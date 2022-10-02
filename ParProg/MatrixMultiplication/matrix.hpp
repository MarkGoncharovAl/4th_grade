#include <algorithm>
#include <array>
#include <iostream>

template <typename T , int X , int Y> class Matrix
{
public:
  Matrix (std::initializer_list<T> arr)
  {
    auto cur_iter = arr.begin ();
    for (int i = 0; i < X; ++i)
    {
      auto next = std::next (cur_iter , Y);
      std::copy (cur_iter , next , data_ + i * Y);
      cur_iter = next;
    }
  }
  Matrix () = default;
  Matrix (const Matrix& that)
  {
    std::copy (that.data_ , that.data_ + X * Y , data_);
  }
  Matrix (Matrix&& that) = default;

  void dump (std::string str = "Dumping matrix") const
  {
    std::cout << str << "\n";
    for (int i = 0; i < X; ++i)
    {
      for (int j = 0; j < Y; ++j)
      {
        std::cout << data_[i * Y + j] << " ";
      }
      std::cout << "\n";
    }
  }

  bool operator==(const Matrix<T , X , Y>& that) const
  {
    return std::equal (data_ , data_ + X * Y , that.data_);
  }

  Matrix<T , X , Y> operator + (const Matrix<T , X , Y>& that) const
  {
    Matrix<T , X , Y> out (that);
    std::transform (data_ , data_ + X * Y , out.data_ , out.data_ , std::plus<> ());
    return out;
  }

  inline T* data ()
  {
    return data_;
  }
  inline T* operator[](int x)
  {
    return data_ + x * Y;
  }
  inline const T* operator[](int x) const
  {
    return data_ + x * Y;
  }

private:
  T data_[X * Y] = {};
};