#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include "nucleusDetail.hpp"
#define MAXINT 1 << 31
#define callocsize 32
#define STEP 1
#define OWNSIZE 1024
struct index
{
  int code1;
  int code2;
  int code3;
};

struct cell
{
  void* pt;
  int gen;
  bool free;
};

struct table
{
  struct cell** owner;
  int ownersize;
  int cellsize;
};

void* get (struct table* tb, struct index* in)
{
  if (in->code1 >=0 && in->code1 < tb->ownersize && in->code2 >=0 && in->code2 < callocsize) {
    if (tb->owner[in->code1][in->code2].gen == in->code3 && tb->owner[in->code1][in->code2].free == false) {
      errorCode = ErrCode::Non;
      return tb->owner[in->code1][in->code2].pt;
    }
  }
  errorCode = ErrCode::GetZero;
  return nullptr;
}
struct index createNew (struct table* tb)
{
  for (int i = 0; i < tb->ownersize; i++)
  {
    for (int j = 0; j < callocsize; j++)
    {
        if (tb->owner[i][j].free == true) {
          tb->owner[i][j].free = false;
          tb->owner[i][j].gen+=1;
          return {i, j, tb->owner[i][j].gen};
        }
    } 
  }
  tb->ownersize+=1;
  short * tt = (short*)(calloc (callocsize , tb->cellsize));
  for (int i = 0; i < callocsize; i+=STEP) {
    tb->owner[tb->ownersize-1][i] = {tt+i*tb->cellsize,0,true};}
  tb->owner[tb->ownersize-1][0].free = false;
  return {tb->ownersize-1, 0, 0};
}

struct cell** init_owner()
{
  struct cell** owner = (struct cell**)malloc(OWNSIZE * sizeof(struct cell));
  for (int i = 0; i < OWNSIZE; i++)
    owner[i] = (struct cell*)calloc(callocsize, sizeof(struct cell));
  return owner;
}

struct table init_table(int cellsize)
{
  return {init_owner(), 0, cellsize};
}

ErrCode Free(struct table* tb, struct index* in)
{
  int code1 = in->code1;
  int code2 = in->code2;
  int code3 = in->code3;
  auto it = tb->owner[code1][code2];
  if (code1 < 0 || code1 >= tb->ownersize || code2 < 0 || code2 >= callocsize)
    return errorCode = ErrCode::FreeZero;
  if (it.gen != code3)
    return errorCode = ErrCode::WrongGen;
  if (it.free == true)
    return errorCode = ErrCode::FreeZero;
  tb->owner[code1][code2].free = true;
  return errorCode = ErrCode::Non;
}
