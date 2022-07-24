#pragma once

#include <IQuery.hpp>
#include <vector>

namespace level {

template <typename TData> class IIndexer {
public:
  virtual ~IIndexer() {}

  virtual bool supportsQuery(const IQuery &query) = 0;

  virtual std::vector<unsigned int> getIndexList(const IQuery &query) = 0;
  virtual void addData(const TData &entry, unsigned int index) = 0;
  virtual void removeData(const TData &entry, unsigned int index) = 0;
};
} // namespace level