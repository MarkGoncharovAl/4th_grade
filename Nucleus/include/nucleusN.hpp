#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include "nucleusDetail.hpp"
#define MAXINT 1 << 31
#define callocsize 1024
#define OWNSIZE 1024

#ifdef DEBUG
#define STEP 2
#endif
#ifndef DEBUG
#define STEP 1
#endif

extern ErrCode errorCode;

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

ErrCode destroy(struct table* tb)
{
  for (int i = 0; i < tb->ownersize; i++) {
    free(tb->owner[i][0].pt);
  }
  free(tb->owner);
  return ErrCode::Non;
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
  char * tt = (char*)(malloc (callocsize * tb->cellsize));
  for (int i = 0; i < callocsize*tb->cellsize; i++){
    tt[i] = (int)0;
        // std::cout << "calloc[" << i << "] = " << (int)tt[i] << std::endl;
    // std::cout << (int*)(tt + i) << std::endl;
  }
  // std::cout << "CREATENEW " << tt << std::endl;
  for (int i = 0; i < callocsize; i+=1) {
    tb->owner[tb->ownersize-1][i] = {tt+i*tb->cellsize*STEP,0,true};}
  tb->owner[tb->ownersize-1][0].free = false;
  errorCode = ErrCode::Non;
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

#ifdef DEBUG
ErrCode check_bounds (struct table* tb)
{
  for (int i = 0; i < tb->ownersize; i++) {
    for (int j = tb->cellsize; j < callocsize*tb->cellsize; j++) {
        // std::cout << "mem[" << j << "] = " << (int)*((char*)tb->owner[i][0].pt + j) << std::endl;
        // std::cout << (int*)((char*)tb->owner[i][0].pt + j) << std::endl;
      if (*((char*)tb->owner[i][0].pt + j) != (int)(0)) {
        // std::cout << "OUTOFBOUDS\n";
        return ErrCode::OutOfBounds;}
        if (j % tb->cellsize == tb->cellsize - 1)
          j+= tb->cellsize;
    }
  }
  // std::cout << "NOTOUTOFBOUDS\n";
  return ErrCode::Non;
}
#endif

struct index check_leaks(struct table* tb)
{
  for (int i = 0; i < tb->ownersize; i++)
  {
    for (int j = 0; j < callocsize; j++)
    {
      if (tb->owner[i][j].free == false) {
        errorCode = ErrCode::Leaks;
        return {i,j,tb->owner[i][j].gen};}
    }
  }
  errorCode = ErrCode::Non;
  return {-1, -1, -1};
}