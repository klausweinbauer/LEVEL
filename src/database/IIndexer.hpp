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
 * @file IIndexer.hpp
 * @author Klaus Weinbauer
 * @brief Generic indexer interface used to implement database logic. Calls to
 * this interface made by the database are already thread-safe. Consumers do not
 * need to implement synchronization by themselves.
 * @version 0.1
 * @date 2022-07-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <IQuery.hpp>
#include <memory>
#include <vector>

namespace level {

/**
 * @brief Generic indexer interface used to implement database logic. Calls to
 * this interface made by the database are already thread-safe. Consumers do not
 * need to implement synchronization by themselves.
 *
 * @tparam T Data type stored in the database.
 */
template <typename T> class IIndexer {
public:
  virtual ~IIndexer() {}

  /**
   * @brief Checks if this instance supports a specific query.
   *
   * @param query Query to verify support.
   * @return true Indexer supports this query.
   * @return false Indexer does not support this query.
   */
  virtual bool supportsQuery(std::shared_ptr<IQuery> query) const = 0;

  /**
   * @brief Get a list of indices which meet the constraints made by the query.
   *
   * @param query Query to select indices.
   * @return std::vector<unsigned int> List of indices which meet the query
   * constraints.
   */
  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<IQuery> query) const = 0;

  /**
   * @brief This method is used by the database to inform indexers about a new
   * data entry.
   *
   * @param entry New data element.
   * @param index Index of the data element.
   */
  virtual void addData(const T &entry, unsigned int index) = 0;

  /**
   * @brief This method is used by the database to inform indexers about an
   * updated data entry.
   *
   * @param entry Updated data element.
   * @param index Index of the data element.
   */
  virtual void updateData(const T &entry, unsigned int index) = 0;

  /**
   * @brief This method is used by the database to inform indexers about a
   * deleted data entry.
   *
   * @param entry Deleted data element.
   * @param index Index of the data element.
   */
  virtual void removeData(const T &entry, unsigned int index) = 0;

  /**
   * @brief This method is used by the database to check if a given entry is
   * valid for the given index.
   *
   * @param entry The data element to check.
   * @param index The index where the database thinks this element is.
   * @return true If the data element and index combination is valid with the
   * current indexer state.
   * @return false If the data element and index combination do not exist (are
   * invalid) in the current indexer state.
   */
  virtual bool isValid(const T &entry, unsigned int index) const = 0;
};

} // namespace level