#include "nucleus.h"
#include "nucleusDetail.h"
#include <stdio.h>
#include <stdlib.h>

static ErrCode errorCode = ErrCode::Non;

void dump (const char str[], Level curLvl) {
  if (LEVEL <= (int)curLvl)
    printf("%s\n", str);
}

enum ErrCode Check ()
{
  CheckCode (errorCode);
  return errorCode;
}

void CheckCode (ErrCode err)
{
  switch (err)
  {
  case ErrCode::Non:
    dump ("Check(): No errors occured" , Level::Debug);
    break;
  case ErrCode::AllocZero:
    dump ("Check(): Allocated zero memory" , Level::Info);
    break;
  case ErrCode::FreeZero:
    dump ("Check(): Tried to free nullptr" , Level::Info);
    break;
  case ErrCode::ReadZero:
    dump ("Check(): Tried to read from nullptr!" , Level::Info);
    break;
  case ErrCode::WriteZero:
    dump ("Check(): Tried to write to nullptr!" , Level::Info);
    break;
  default:
    dump ("Check(): There is no appropriate processing error! Check ErrCode enum and process it correctly" , Level::Info);
    break;
  }
}

reg Alloc ()
{
  reg out = calloc (1 , 4);
  if (out) {
    errorCode = ErrCode::Non;
    return out;
  }
  errorCode = ErrCode::AllocZero;
  dump ("Alloc(): Allocation wasn't successful" , Level::Info);
  return NULL;
}

ErrCode Free (reg curReg)
{
  if (curReg) {
    errorCode = ErrCode::Non;
    free (curReg);
  }
  else
    errorCode = ErrCode::FreeZero;
  return errorCode;

  errorCode = ErrCode::FreeZero;
  dump ("Free(): Tried to free from nullptr memory" , Level::Info);
}

int Read (reg curReg)
{
  if (curReg) {
    errorCode = ErrCode::Non;
    return *(int*)curReg;
  }

  dump ("Read(): Tried to read memory from nullptr", Level::Info);
  errorCode = ErrCode::ReadZero;
  return (int)errorCode;
}

ErrCode Write (reg curReg , int data)
{
  if (curReg) {
    errorCode = ErrCode::Non;
    *(int*)curReg = data;
    return errorCode;
  }

  dump ("Write(): Tried to write to nullptr memory" , Level::Info);
  errorCode = ErrCode::WriteZero;
  return errorCode;
}