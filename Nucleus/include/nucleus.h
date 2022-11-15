#pragma once
#include <memory.h>

using reg = void*;

/* ------------------------------------------
  Error code processed by @check@ functions
------------------------------------------ */
enum class ErrCode : int
{
  Non = 0 ,
  AllocZero,
  FreeZero,
  ReadZero,
  WriteZero
};

/* ------------------------------------------
  Create new region 32 bites
------------------------------------------ */
reg Alloc();

/* ------------------------------------------
  Free region <curReg>
------------------------------------------ */
void Free(reg curReg);

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