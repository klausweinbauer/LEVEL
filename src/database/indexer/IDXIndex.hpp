// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <Indexer.hpp>
#include <QRYIndex.hpp>

namespace level {

template <typename T> class IDXIndex : public Indexer<T, QRYIndex> {

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