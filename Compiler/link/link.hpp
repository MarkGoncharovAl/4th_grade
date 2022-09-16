#pragma once 
#include "common.h"

extern "C" int initWindow();
extern "C" int closeWindow();
extern "C" int updateWindow(const float pixels[SzW][SzH]);