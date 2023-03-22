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

/**
 * @file Indexer.hpp
 * @author Klaus Weinbauer
 * @brief Base class for indexer implementations. Override addData(),
 * updateData(), removeData() and isValid() according to the logic as needed.
 * @version 0.1
 * @date 2022-07-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <DBException.hpp>
#include <IIndexer.hpp>

namespace level {

/**
 * @brief Base class for indexer implementations. Override addData(),
 * updateData(), removeData() and isValid() according to the logic as needed.
 *
 * @tparam TData Data type stored in the database.
 * @tparam TQuery Type of the query that this indexer supports. This type must
 * implement the IQuery interface.
 */
template <typename TData, typename TQuery>
class Indexer : public IIndexer<TData> {

public:
  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<TQuery> query) const = 0;

  virtual bool supportsQuery(std::shared_ptr<IQuery> query) const override {
    return dynamic_cast<TQuery *>(query.get());
  };

  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<IQuery> query) const override {
    auto castedQuery = dynamic_cast<TQuery *>(query.get());
    if (!castedQuery) {
      throw DBException(ERR_INVALID_ARG,
                        "The query is not derived from the defined query type "
                        "used in this indexer.");
    }
    std::shared_ptr<TQuery> derivedQuery =
        std::static_pointer_cast<TQuery>(query);
    return getIndexList(derivedQuery);
  }

  virtual void addData(const TData &entry, unsigned int index) {}

  virtual void updateData(const TData &entry, unsigned int index) {}

  virtual void removeData(const TData &entry, unsigned int index) {}

  virtual bool isValid(const TData &entry, unsigned int index) const {
    return true;
  }
};

} // namespace level
