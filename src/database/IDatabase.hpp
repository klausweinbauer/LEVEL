/**
 * @file IDatabase.hpp
 * @author Klaus Weinbauer
 * @brief Generic database interface.
 * @version 0.1
 * @date 2022-07-26
 *
 * @copyright Copyright (c) 2022
 *
 * @details The database is responsible for managing the stored resources.
 * Database logic is implemented as indexers (IIndexer) and added to the
 * database as needed by the provided method. Requests to retrieve data from the
 * database are made by queries (IQuery). These queries are then forwarded to
 * indexers that can handle the specific type. The database returns the selected
 * entries by the indexer as a DBView list which safely encapsulates the
 * requested resource.
 */

#pragma once

#include <IIndexer.hpp>
#include <IQuery.hpp>
#include <vector>

namespace level {

/**
 * @brief Generic database interface.
 *
 * @details The database is responsible for managing the stored resources.
 * Database logic is implemented as indexers (IIndexer) and added to the
 * database as needed by the provided method. Requests to retrieve data from the
 * database are made by queries (IQuery). These queries are then forwarded to
 * indexers that can handle the specific type. The database returns the selected
 * entries by the indexer as a DBView list which safely encapsulates the
 * requested resource.
 *
 * @tparam T Data type to store in the database.
 */
template <typename T> class IDatabase {
public:
  virtual ~IDatabase() {}

  /**
   * @brief Add an indexer to this database. The caller transfers the ownership
   * of the indexer to the database.
   *
   * @param indexer Indexer to add to the database.
   */
  virtual void addIndexer(std::unique_ptr<IIndexer<T>> indexer) = 0;

  /**
   * @brief Returns the number of elements in the database. The returned value
   * is not guaranteed to be still valid when read.
   *
   * @return unsigned int Probably the number of elements in the database.
   */
  virtual unsigned int count() = 0;

  /**
   * @brief Insert a new element by value. The inserted type must implement a
   * copy constructor.
   *
   * @param entry Value to insert.
   * @return DBView<T> A view to the newly inserted value.
   */
  virtual DBView<T> insert(T entry) = 0;

  /**
   * @brief Insert a new element by reference. The caller transfers ownership
   * of the entry to the database.
   *
   * @param entry Value to insert.
   * @return DBView<T> A view to the newly inserted value.
   */
  virtual DBView<T> insert(std::unique_ptr<T> entry) = 0;

  /**
   * @brief Get views on elements based on a query.
   *
   * @param query Query to select data objects.
   * @return std::vector<DBView<T>> Vector of views on the selected entries.
   */
  virtual std::vector<DBView<T>> get(std::shared_ptr<IQuery> query) = 0;

  /**
   * @brief Deletes a database entry by a view.
   *
   * @param view A view of the entry to delete.
   * @return true Successfully deleted the entry.
   * @return false Failed to delete the entry.
   */
  virtual bool remove(DBView<T> &view) = 0;

  /**
   * @brief Deletes a database entry by a view.
   *
   * @param view A view of the entry to delete.
   * @return true Successfully deleted the entry.
   * @return false Failed to delete the entry.
   */
  virtual bool remove(DBView<T> &&view) = 0;
};

} // namespace level