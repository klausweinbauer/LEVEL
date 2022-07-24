#pragma once

#include <IIndexer.hpp>
#include <IQuery.hpp>
#include <vector>

namespace level {

template <typename T> class IDatabase {
public:
  virtual ~IDatabase(){};

  virtual void addIndexer(std::shared_ptr<IIndexer<T>> indexer) = 0;
  virtual int count() = 0;
  virtual DBView<T> insert(T entry) = 0;
  virtual DBView<T> insert(std::unique_ptr<T> entry) = 0;
  virtual std::vector<DBView<T>> get(const IQuery &query) = 0;
  virtual bool remove(DBView<T> &view) = 0;
  virtual bool remove(int index) = 0;
};

} // namespace level