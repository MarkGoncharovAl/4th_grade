#include "math/math.h"
#include <unistd.h>

int main ()
{
  short pixels[SzW][SzH];

  initPixels (pixels);
  initWindow ();

  while (1)
  {
    updatePixels (pixels);
    if (flush () == 1) // we have to exit
      break;
  }
  closeWindow ();
}