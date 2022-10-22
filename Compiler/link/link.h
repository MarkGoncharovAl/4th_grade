#pragma once
#include "common.h"

int initWindow();
int closeWindow();

// Interface for this task
int putPixel(int x, int y, short heat);
int flush();