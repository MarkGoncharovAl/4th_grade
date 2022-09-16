#pragma once 
#include "common.h"

extern "C" int initWindow ();
extern "C" int closeWindow ();

// Interface for this task
extern "C" int putPixel (int x , int y , float heat);
extern "C" int flush ();