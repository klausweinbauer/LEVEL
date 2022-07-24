#pragma once

#include <IQuery.hpp>
#include <memory>
#include <vector>

namespace level {

template <typename T> class IIndexer {
public:
  virtual ~IIndexer() {}

  virtual bool supportsQuery(std::shared_ptr<IQuery> query) = 0;

  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<IQuery> query) = 0;
  virtual void addData(const T &entry, unsigned int index) = 0;
  virtual void removeData(const T &entry, unsigned int index) = 0;
};
} // namespace level