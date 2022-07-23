#pragma once

#include <IIndexer.hpp>

namespace level {

template <typename T> class Indexer : public IIndexer<T> {
private:
  const int _queryType;

public:
  Indexer(int queryType) : _queryType(queryType){};
  virtual ~Indexer(){};

  virtual int getQueryType() const { return _queryType; };

  virtual std::vector<unsigned int> getIndexList(const IQuery &query) {
    return std::vector<unsigned int>();
  };

  virtual void valueChanged(const T *const entry) {}
};
} // namespace level