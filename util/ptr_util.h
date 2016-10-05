// Library of utility functions to deal with pointers.
// Copyright 2016 Hieu Le.

#ifndef UTIL_PTR_UTIL_H__
#define UTIL_PTR_UTIL_H__

#include <memory>
#include <utility>

namespace util {

// Constructs an object of type T and wraps it in a std::unique_ptr.
template <typename T, typename ...Args>
inline std::unique_ptr<T> make_unique(Args&& ...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace util

#endif  // UTIL_PTR_UTIL_H__
