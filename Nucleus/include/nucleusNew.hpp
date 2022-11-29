#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include "nucleusDetail.hpp"
#define MAXINT 1 << 31
template <typename T>
class managerMemory_t
{
  public:
  T* get (int code);
  ErrCode Free (int code);
  #ifdef DEBUG
  ErrCode check_bounds ();
  #endif
  std::pair<int , T*> createNew ();
  ~managerMemory_t ();

  private:
  // Used by user pointer <-> id code
  std::unordered_map<int , T*> globalTable;

  // Not yet used allocated memory
  std::vector<T*> empty;

  std::vector<T*> owner;

  static constexpr int callocSize = 100;
};

template <typename T>
T* managerMemory_t<T>::get (int code)
{
  auto it = globalTable.find (code);
  if (it != globalTable.end ())
  {
    errorCode = ErrCode::Non;
    return it->second;
  }
  errorCode = ErrCode::GetZero;
  return nullptr;
}
template <typename T>
std::pair<int , T*> managerMemory_t<T>::createNew ()
{
  // Index to give user
  static int lastIndex = 0;

  // Prepare common data for both cases
  T* toUserPt = nullptr;
  ++lastIndex;

  // Allocate new memory if necessary.
  if (empty.empty ())
  {
    void * tt = (calloc (callocSize , sizeof (T)));
    #ifdef DEBUG
    char * t = static_cast<char*> (tt);
    for (int i = 0; i < callocSize*sizeof(T); i++)
      t[i] = static_cast<char>(255);
    #endif
    T* newMemory = static_cast<T*>(tt);
    owner.push_back (newMemory);
    for (int i = 0; i < callocSize; i+=2) {
      empty.push_back (&(newMemory[i]));}
  }

  // Return final result and save to hash map data
  toUserPt = empty.back ();
  empty.pop_back ();
  auto toUser = std::make_pair (lastIndex - 1 , toUserPt);
  globalTable.insert (toUser);
  return toUser;
}
template <typename T>
ErrCode managerMemory_t<T>::Free (int code)
{
  auto it = globalTable.find (code);
  if (it == globalTable.end ())
    return errorCode = ErrCode::FreeZero;
  empty.push_back(it->second);
  globalTable.erase(it);
  return errorCode = ErrCode::Non;
}
#ifdef DEBUG
template <typename T>
ErrCode managerMemory_t<T>::check_bounds ()
{
  for (void* mem: owner) {
    char* t = static_cast<char*>(mem);
    for (int i = sizeof(T) + 1; i < (callocSize)*sizeof(T); i++) {
        // std::cout << "mem[" << i << "] = " << (int)t[i] << std::endl;
        // std::cout << (int*)(t+i) << std::endl;
      if (t[i] != static_cast<char>(255)) {
        // std::cout << "OUTOFBOUDS\n";
        return ErrCode::OutOfBounds;}
        if (i % sizeof(T) == sizeof(T) - 1)
          i+= sizeof(T);
    }
  }
  // std::cout << "NOTOUTOFBOUDS\n";
  return ErrCode::Non;
}
#endif
template <typename T>
managerMemory_t<T>::~managerMemory_t ()
{
  for (auto mem : owner)
    free (mem);
}