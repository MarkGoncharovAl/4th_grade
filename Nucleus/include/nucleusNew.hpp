#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include "nucleusDetail.hpp"

template <typename T>
class managerMemory_t
{
  public:
  T* get (int code);
  ErrCode Free (int code);
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
  if (it == globalTable.end ())
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
    T* newMemory = static_cast<T*>(calloc (callocSize , sizeof (T)));
    owner.push_back (newMemory);
    for (int i = 0; i < callocSize; ++i)
      empty.push_back (&newMemory[i]);
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
template <typename T>
managerMemory_t<T>::~managerMemory_t ()
{
  for (auto mem : owner)
    free (mem);
}