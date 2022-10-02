#pragma once
#include "../link/link.h"

int initPixels (short pixels[SzW][SzH]);
int updatePixels (short pixels[SzW][SzH]);

void check_impl (int err_code , const char* file , const unsigned line);
#define check(statement) check_impl(statement, __FILE__, __LINE__);

#define Speed 0.24
#define Iterations 25