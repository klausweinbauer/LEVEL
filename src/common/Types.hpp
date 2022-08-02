#pragma once

#include <typeinfo>

namespace level {

template <typename T> struct TypeName {
  static const char *get() { return typeid(T).name(); }
};

} // namespace level