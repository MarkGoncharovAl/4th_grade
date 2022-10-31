#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <omp.h>
#include <vector>

using data_t = std::vector<double>;

void ccopy(int n, data_t &x, data_t &y);
void cfft2(int n, data_t &x, data_t &y, data_t &w, double sgn);
void cffti(int n, data_t &w);
double ggl(double *ds);
void step(int n, int mj, double *a, double *b, double *c, double *d, double *w,
          double sgn);
double cpu_time();

int main() {
  int nits = 10000;
  double seed = 331.0;
  int n = 2;
  for (int ln2 = 1; ln2 <= 22; ln2++, n = 2 * n) {
    std::vector<double> w(n);
    std::vector<double> x(2 * n);
    std::vector<double> y(2 * n);
    std::vector<double> z(2 * n);

    bool first = true;
    for (int icase = 0; icase < 2; icase++) {
      for (int i = 0; i < 2 * n; i = i + 2) {
        int z0 = 0.0;
        int z1 = 0.0;
        x[i] = z0;
        z[i] = z0;
        x[i + 1] = z1;
        z[i + 1] = z1;
      }
      cffti(n, w);
      if (first) {
        cfft2(n, x, y, w, 1.0);
        cfft2(n, y, x, w, -1.0);
        //
        //  Results should be same as initial multiplied by N.
        //
        double fnm1 = 1.0 / (double)n;
        double error = 0.0;
        for (int i = 0; i < 2 * n; i = i + 2) {
          error +=
              pow(z[i] - fnm1 * x[i], 2) + pow(z[i + 1] - fnm1 * x[i + 1], 2);
        }
        error = sqrt(fnm1 * error);
        std::cout << "  " << std::setw(12) << n;
        first = false;
      } else {
        double ctime1 = cpu_time();
        for (int it = 0; it < nits; it++) {
          cfft2(n, x, y, w, 1.0);
          cfft2(n, y, x, w, -1.0);
        }
        double ctime2 = cpu_time();
        double ctime = ctime2 - ctime1;

        double flops = 2.0 * (double)nits * (5.0 * (double)n * (double)ln2);
        double mflops = flops / 1.0E+06 / ctime;
        std::cout << "  " << std::setw(12) << ctime / (double)(2 * nits)
                  << "\n";
      }
    }
    if ((ln2 % 4) == 0)
      nits = nits / 10;
    if (nits < 1)
      nits = 1;
  }

  return 0;
}

void ccopy(int n, data_t &x, data_t &y) {
  for (int i = 0; i < n; i++) {
    y[i * 2] = x[i * 2];
    y[i * 2 + 1] = x[i * 2 + 1];
  }
}

void cfft2(int n, data_t &x, data_t &y, data_t &w, double sgn) {
  int m = static_cast<int>(log((double)n) / log(1.99));
  int mj = 2;
  //
  //  Toggling switch for work array.
  //
  bool tgle = true;
  step(n, 1, &x[0], &x[(n / 2) * 2], &y[0], &y[mj], w.data(), sgn);

  if (n == 2)
    return;

  for (int j = 0; j < m - 2; j++, mj = mj * 2) {
    if (tgle) {
      step(n, mj, &y[0], &y[(n / 2) * 2], &x[0], &x[mj * 2], w.data(), sgn);
      tgle = false;
    } else {
      step(n, mj, &x[0], &x[(n / 2) * 2], &y[0], &y[mj * 2], w.data(), sgn);
      tgle = true;
    }
  }
  //
  //  Last pass thru data: move y to x if needed
  //
  if (tgle)
    ccopy(n, y, x);

  mj = n / 2;
  step(n, mj, &x[0], &x[(n / 2) * 2], &y[0], &y[mj * 2], w.data(), sgn);
  return;
}

void cffti(int n, data_t &w) {
  constexpr double pi = 3.141592653589793;
  int n2 = n / 2;
  int aw = 2.0 * pi / ((double)n);

  for (int i = 0; i < n2; i++) {
    double arg = aw * ((double)i);
    w[i * 2] = cos(arg);
    w[i * 2 + 1] = sin(arg);
  }
}

double cpu_time() { return (double)clock() / (double)CLOCKS_PER_SEC; }

double ggl(double *seed)

{
  double d2 = 0.2147483647e10;
  double t;
  double value;

  t = *seed;
  t = fmod(16807.0 * t, d2);
  *seed = t;
  value = (t - 1.0) / (d2 - 1.0);

  return value;
}

void step(int n, int mj, double *a, double *b, double *c, double *d, double *w,
          double sgn) {
  std::array<double, 2> wjw;
  int mj2 = 2 * mj;
  int lj = n / mj2;

  for (int j = 0; j < lj; j++) {
    int jw = j * mj;
    int ja = jw;
    int jb = ja;
    int jc = j * mj2;
    int jd = jc;

    wjw[0] = w[jw * 2 + 0];
    wjw[1] = w[jw * 2 + 1];

    if (sgn < 0.0)
      wjw[1] = -wjw[1];

    for (int k = 0; k < mj; k++) {
      c[(jc + k) * 2 + 0] = a[(ja + k) * 2 + 0] + b[(jb + k) * 2 + 0];
      c[(jc + k) * 2 + 1] = a[(ja + k) * 2 + 1] + b[(jb + k) * 2 + 1];

      double ambr = a[(ja + k) * 2 + 0] - b[(jb + k) * 2 + 0];
      double ambu = a[(ja + k) * 2 + 1] - b[(jb + k) * 2 + 1];

      d[(jd + k) * 2 + 0] = wjw[0] * ambr - wjw[1] * ambu;
      d[(jd + k) * 2 + 1] = wjw[1] * ambr + wjw[0] * ambu;
    }
  }
}

void showTime() {
  constexpr int timeSize = 40;
  static char time_buffer[timeSize];
  const struct tm *tm;

  time_t now;
  now = time(NULL);
  tm = localtime(&now);
  strftime(time_buffer, timeSize, "%d %B %Y %I:%M:%S %p", tm);
  std::cout << time_buffer << "\n";
}