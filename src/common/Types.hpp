#pragma once

#include <typeinfo>

namespace level {

#define BYTE unsigned char

template <typename T> struct TypeName {
  static const char *get() { return typeid(T).name(); }
};

} // namespace level