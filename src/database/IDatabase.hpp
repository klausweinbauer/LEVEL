#pragma once

#include <IQuery.hpp>
#include <vector>

namespace level {

template <typename T> class IDatabase {
public:
  virtual ~IDatabase(){};

  virtual int count() = 0;
  virtual DBView<T> insert(T *entry) = 0;
  virtual std::vector<DBView<T>> get(IQuery query) = 0;
  virtual int remove(IQuery query) = 0;
};

} // namespace level