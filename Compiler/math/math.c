#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

float new_pixels[SzW][SzH];

int initPixels (float pixels[SzW][SzH])
{
  for (int i = 0; i < SzH; ++i)
  {
    pixels[0][i] = 100.0;
    new_pixels[0][i] = 100.0;
  }

  for (int i = 0; i < SzW; ++i)
  {
    pixels[i][0] = 50.0;
    new_pixels[i][0] = 50.0;
  }

  for (int i = 1; i < SzW; ++i)
  {
    for (int j = 1; j < SzH; ++j)
    {
      pixels[i][j] = 0.0;
      new_pixels[i][j] = 0.0;
    }
  }
  return 0;
}

int updatePixels (float pixels[SzW][SzH])
{
  for (int k = 0; k < Iterations; ++k)
  {
    for (int i = 1; i < SzW - 1; ++i)
    {
      for (int j = 1; j < SzH - 1; ++j)
        new_pixels[i][j] = pixels[i][j] + Speed *
        (pixels[i - 1][j] + pixels[i][j - 1] + pixels[i + 1][j] + pixels[i][j + 1] - 4 * pixels[i][j]);
    }

    for (int i = 1; i < SzW - 1; ++i)
      for (int j = 1; j < SzH - 1; ++j)
        pixels[i][j] = new_pixels[i][j];
  }
  return 0;
}

void check_impl (int err_code , const char* file , const unsigned line)
{
  if (err_code)
  {
    printf ("ERROR: in file %s, line %u" , file , line);
    abort ();
  }
}