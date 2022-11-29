#pragma once
/* ------------------------------------------
  Error code processed by @check@ functions
------------------------------------------ */
enum class ErrCode: int
{
  Non = 0 ,
  GetZero ,
  AllocZero ,
  FreeZero ,
  ReadZero ,
  WriteZero ,
  OutOfBounds ,
  WrongGen ,
  Leaks
};

ErrCode Check();


enum class Level: int
{
  Debug = 0 ,
  Info = 1 ,
  Release = 2
};