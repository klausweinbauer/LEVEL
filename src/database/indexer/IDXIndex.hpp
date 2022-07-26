#pragma once

#include <Indexer.hpp>
#include <QRYIndex.hpp>

namespace level {

template <typename T> class IDXIndexer : public Indexer<T, QRYIndex> {

public:
  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<QRYIndex> query) const {
    std::vector<unsigned int> indices;
    for (unsigned int i = query->firstIndex(); i < query->lastIndex(); i++) {
      indices.push_back(i);
    }
    return indices;
  }
};

} // namespace level