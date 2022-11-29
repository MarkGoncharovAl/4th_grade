#pragma once
#include "common.h"

#ifndef DEBUG
#ifndef INFO
#ifndef RELEASE
constexpr int LEVEL = (int)Level::Release;
#endif
#endif
#endif

#ifdef DEBUG
constexpr int LEVEL = (int)Level::Debug;
#endif
#ifdef INFO
constexpr int LEVEL = (int)Level::Info;
#endif
#ifdef RELEASE
constexpr int LEVEL = (int)Level::Release;
#endif

void dump (const char str [] , Level curLvl);