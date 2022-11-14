#pragma once

#include <iostream>
#include <memory>
#include <concepts>
#include <list>

#ifndef LEVEL
#define LEVEL Release
#endif

enum class Level : int {
  Debug = 0,
  Info = 1,
  Release = 2
};
inline int getLevel(Level lvl) {
  return static_cast<int>(lvl);
}
inline int checkLevel () {
  return static_cast<int>(Level::LEVEL);
}
inline void dump(const std::string &str, Level lvl) {
  if (getLevel(lvl) >= checkLevel())
    std::cout << str << "\n";
}

template <std::default_initializable T>
class SafePtr {
  public:
    SafePtr(T* data) :
      data_(data) {}    
    SafePtr (SafePtr const& that):
      data_ (that.data_) {}
    SafePtr (SafePtr &&that):
      data_ (std::move(that.data_)) {}
    T operator *() const { 
      if (!data_)
        return *data_; 
      dump("Get data of pointer wasn't safe", Level::Info);
      return T{};
    }
    T* operator -> ()
    {
      if (!data_)
        return data_;
      dump ("Get method of pointer wasn't safe" , Level::Release);
      return nullptr;
    }
    const T* get() const { 
      if (!data_)
        return data_; 
      dump ("Get pointer wasn't safe" , Level::Info);
      return nullptr;
    }
  private:
    T* data_;
};

template <typename T>
class Region {
  public:
    template<typename ... Args>
    SafePtr<T> alloc (Args && ... args) {
      data_.emplace_back (std::make_unique<T> (std::forward<Args> (args)...));
      return SafePtr<T>(data_.back().get());
    }
    void free (SafePtr<T> s_ptr)
    {
      const T* ptr = s_ptr.get();
      if (!ptr) {
        dump("Free function has nullptr input", Level::Info);
        return;
      }

      auto it = std::find_if(data_.cbegin(), data_.cend(), [ptr]
        (const std::unique_ptr<T> &elem) {
          return elem.get() == ptr;
        });
      if (it != data_.cend())
        data_.erase(it);
      else
        dump("Free function has pointer that was already deleted", Level::Debug);
    }
  private:
    std::list<std::unique_ptr<T>> data_;
};