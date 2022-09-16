#include "math/math.h"
#include <unistd.h>

int main ()
{
  float pixels[SzW][SzH];

  check (initPixels (pixels));
  check (initWindow ());

  while (1)
  {
    check (updatePixels (pixels));
    if (flush () == 1) // we have to exit
      break;
  }
  check (closeWindow ());
}