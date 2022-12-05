#pragma once
#include <memory.h>
#include "common.h"

using reg = void*;

/* ------------------------------------------
  Create new region 32 bites
------------------------------------------ */
reg Alloc();

/* ------------------------------------------
  Free region <curReg>
------------------------------------------ */
ErrCode Free(reg *curReg);

/* ------------------------------------------
  Get data from region <curReg>
------------------------------------------ */
int Read(reg curReg);

/* ------------------------------------------
  Write <data> to region <curReg>
------------------------------------------ */
ErrCode Write (reg curReg, int data);

/* ------------------------------------------
  Check for global error state
------------------------------------------ */
ErrCode Check();

/* ------------------------------------------
  Check for local <err> error state
------------------------------------------ */
void CheckCode(ErrCode err);

/* ------------------------------------------
  Get pointer to user or nullptr if data
  is unavailable
------------------------------------------ */
void* Get(int code);