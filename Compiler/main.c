#include "link/link.h"
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
    if (updateWindow (pixels) == 1)
      break;
  }
  check (closeWindow ());
}