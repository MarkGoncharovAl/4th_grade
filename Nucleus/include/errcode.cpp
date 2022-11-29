#include "nucleusDetail.hpp"
#include <stdio.h>
void dump (const char str[], Level curLvl) {
  if (LEVEL <= (int)curLvl)
    printf("%s\n", str);
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
    dump ("Check(): Tried to read from nullptr" , Level::Info);
    break;
  case ErrCode::WriteZero:
    dump ("Check(): Tried to write to nullptr!" , Level::Info);
    break;
  case ErrCode::OutOfBounds:
    dump ("Check(): Tried to write out of bounds" , Level::Info);
    break;
  case ErrCode::WrongGen:
    dump ("Check(): Tried to get wrong gen" , Level::Info);
    break;
  default:
    dump ("Check(): There is no appropriate processing error! Check ErrCode enum and process it correctly" , Level::Info);
    break;
  }
}


enum ErrCode Check ()
{
  CheckCode (errorCode);
  return errorCode;
}