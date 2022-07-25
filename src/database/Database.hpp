/**
 * @file Database.hpp
 * @author Klaus Weinbauer
 * @brief A dynamic, thread- and memory-safe implementation for the IDatabase
 * interface.
 * @version 0.1
 * @date 2022-07-24
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <DBElement.hpp>
#include <IDatabase.hpp>
#include <IIndexer.hpp>
#include <mutex>
#include <unordered_map>

namespace level {

/**
 * @brief A dynamic, thread- and memory-safe implementation for the IDatabase
 * interface.
 *
 * @tparam T Datatype to store in the database.
 */
template <typename T> class Database : public IDatabase<T> {
private:
  unsigned int _size;
  DBElement<T> **_data;
  std::mutex _dataLock;
  std::vector<std::unique_ptr<IIndexer<T>>> _indexer;
  std::mutex _indexerLock;
  std::vector<unsigned int> _freeIndices;
  std::mutex _freeIndicesLock;

  /**
   * @brief Get a free element. The returned element is not locked. This is safe
   * because only the database knows about the element at this time.
   *
   * @return DBElement<T>*
   */
  DBElement<T> *getFreeElement() {
    std::scoped_lock lock(_dataLock, _freeIndicesLock);
    unsigned int index;
    if (_freeIndices.size() > 0) {
      index = _freeIndices.back();
      _freeIndices.pop_back();
    } else {
      index = _size;
      _size += 1;
      _data = (DBElement<T> **)realloc(_data, _size * sizeof(DBElement<T> *));
      _data[index] = new DBElement<T>(index, this);
    }
    return _data[index];
  }

  /**
   * @brief Get element by index
   *
   * @throw DBException if index does not exist.
   *
   * @param index Index of the element.
   * @return DBElement<T>* Entry object.
   */
  DBElement<T> *getElement(unsigned int index) {
    std::lock_guard<std::mutex> guard(_dataLock);

    if (index >= _size) {
      throw DBException(ERR_INDEX_OUT_OF_RANGE,
                        "Element with this index does not exist.");
    }

    return _data[index];
  }

  /**
   * @brief Convert a query into a list of ascending ordered unique indices
   * pointing to the corresponding elements resolved by the registered indexer.
   * REQUIRES _indexerLock AND _dataLock WHEN CALLED.
   *
   * @param query The query to convert.
   * @return std::vector<unsigned int> Ascending ordered unique index list.
   */
  std::vector<unsigned int>
  getIndexListUnlocked(std::shared_ptr<IQuery> query) {
    std::vector<unsigned int> indexList;

    for (auto &&indexer : _indexer) {
      if (indexer->supportsQuery(query)) {
        try {
          auto tmpIndexList = indexer->getIndexList(query);
          for (unsigned int i : tmpIndexList) {
            if (i < _size && _data[i]->hasData()) {
              indexList.push_back(i);
            }
          }
        } catch (const std::exception &) {
          // Ignore indexer exceptions
        }
      }
    }

    std::sort(indexList.begin(), indexList.end());
    auto last = std::unique(indexList.begin(), indexList.end());
    indexList.erase(last, indexList.end());

    return indexList;
  }

  void updateIndexer(const DBElement<T> *const element) {
    std::lock_guard<std::mutex> guard(_indexerLock);

    for (auto &&indexer : _indexer) {
      indexer->updateData(element->data(), element->index());
    }
  }

public:
  Database()
      : _size(0), _data((DBElement<T> **)calloc(0, sizeof(DBElement<T> *))) {}

  virtual ~Database() {
    for (unsigned int i = 0; i < _size; i++) {
      delete _data[i];
      _data[i] = nullptr;
    }

    free(_data);
  }

  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  virtual void addIndexer(std::unique_ptr<IIndexer<T>> indexer) override {
    std::lock_guard<std::mutex> guard(_indexerLock);
    _indexer.push_back(std::move(indexer));
  }

  /**
   * @brief Returns the number of elements in the database. The returned value
   * is not guaranteed to be still valid when read.
   *
   * @return unsigned int Probably the number of elements in the database.
   */
  virtual unsigned int count() override {
    std::scoped_lock lock(_dataLock, _freeIndicesLock);
    return _size - _freeIndices.size();
  }

  /**
   * @brief Insert a new element by value. The inserted type must implement a
   * copy constructor.
   *
   * @param entry
   * @return DBView<T>
   */
  virtual DBView<T> insert(T entry) override {
    return insert(std::make_unique<T>(entry));
  }

  /**
   * @brief Insert a new element by reference. The caller transfers ownership of
   * the entry to the database.
   *
   * @param entry
   * @return DBView<T>
   */
  virtual DBView<T> insert(std::unique_ptr<T> entry) override {
    DBElement<T> *element = getFreeElement();
    element->setData(std::move(entry));
    element->dataChanged = [this](const DBElement<T> *const element) {
      this->updateIndexer(element);
    };

    _indexerLock.lock();
    for (auto &&indexer : _indexer) {
      try {
        indexer->addData(element->data(), element->index());
      } catch (const std::exception &) {
        // Ignore indexer exception
      }
    }
    _indexerLock.unlock();

    DBView<T> view(element);
    return view;
  }

  /**
   * @brief Get views on elements based on a query.
   *
   * @param query Query to select data objects.
   * @return std::vector<DBView<T>> Vector of views on the selected entries.
   */
  virtual std::vector<DBView<T>> get(std::shared_ptr<IQuery> query) override {
    std::vector<DBElement<T> *> elements;

    {
      std::scoped_lock lock(_indexerLock, _dataLock);
      auto indexList = getIndexListUnlocked(query);

      for (unsigned int index : indexList) {
        elements.push_back(_data[index]);
      }
    }

    std::vector<DBView<T>> views;
    for (DBElement<T> *element : elements) {
      views.push_back(DBView<T>(element));
    }

    return views;
  }

  /**
   * @brief Deletes a database entry by a view.
   *
   * @param view A view of the entry to delete.
   * @return true Successfully deleted the entry.
   * @return false Failed to delete the entry.
   */
  virtual bool remove(DBView<T> &view) override {
    view.remove();
    return true;
  }

  /**
   * @brief Deletes a database entry by a view.
   *
   * @param view A view of the entry to delete.
   * @return true Successfully deleted the entry.
   * @return false Failed to delete the entry.
   */
  virtual bool remove(DBView<T> &&view) override {
    view.remove();
    return true;
  }

  /**
   * @brief Deletes a database entry by its index. If the caller does not hold
   * the entry, this call will block until it safely acquired the element for
   * deletion.
   *
   * @throw DBException if index does not point to a valid entry.
   *
   * @param index The index of the entry to delete.
   * @return true Successfully deleted the entry.
   * @return false Failed to delete the entry.
   */
  virtual bool remove(unsigned int index) override {
    DBElement<T> *element = getElement(index);
    if (!element->hasData()) {
      throw DBException(ERR_INDEX_OUT_OF_RANGE,
                        "Element with this index does not exist.");
    }

    bool lockAcquired = false;
    if (element->holdingThread() != std::this_thread::get_id()) {
      element->lock();
      lockAcquired = true;
    }
    _indexerLock.lock();
    for (auto &&indexer : _indexer) {
      try {
        indexer->removeData(element->data(), element->index());
      } catch (const std::exception &) {
        // Ignore indexer exception
      }
    }
    _indexerLock.unlock();
    element->setData(nullptr);
    if (lockAcquired) {
      element->unlock();
    }

    std::lock_guard<std::mutex> freeIndicesGuard(_freeIndicesLock);
    _freeIndices.push_back(element->index());

    return true;
  }
};

} // namespace level