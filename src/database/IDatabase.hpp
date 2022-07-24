#pragma once

#include <IIndexer.hpp>
#include <IQuery.hpp>
#include <vector>

namespace level {

template <typename T> class IDatabase {
public:
  virtual ~IDatabase() {}

  virtual void addIndexer(std::shared_ptr<IIndexer<T>> indexer) = 0;
  virtual unsigned int count() = 0;
  virtual DBView<T> insert(T entry) = 0;
  virtual DBView<T> insert(std::unique_ptr<T> entry) = 0;
  virtual std::vector<DBView<T>> get(std::shared_ptr<IQuery> query) = 0;
  virtual bool remove(DBView<T> &view) = 0;
  virtual bool remove(DBView<T> &&view) = 0;
  virtual bool remove(unsigned int index) = 0;
};

} // namespace level