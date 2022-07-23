#pragma once

#include <IQuery.hpp>
#include <vector>

namespace level {

template <typename T> class IIndexer {
public:
  virtual ~IIndexer(){};

  virtual int getQueryType() const = 0;

  virtual std::vector<unsigned int> getIndexList(const IQuery &query) = 0;
};
} // namespace level