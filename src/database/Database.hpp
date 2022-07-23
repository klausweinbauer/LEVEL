#pragma once

#include <DBElement.hpp>
#include <IDatabase.hpp>
#include <IIndexer.hpp>
#include <mutex>
#include <unordered_map>

namespace level {

template <typename T> class Database : public IDatabase<T> {
private:
  DBElement<T> **_data = nullptr;
  unsigned int _size = 0;
  // DBElement lock must be the inner lock; _lock should be acquired first
  std::mutex _lock;
  std::vector<std::shared_ptr<IIndexer<T>>> _indexer;
  std::unordered_map<T *, unsigned int> _ptrMap;
  std::vector<unsigned int> _emptyIndexList;

  std::vector<unsigned int> getIndexList(const IQuery &query) {
    std::vector<unsigned int> indexList;
    for (std::shared_ptr<IIndexer<T>> indexer : _indexer) {
      if (indexer->getQueryType() == query.getQueryType()) {
        try {
          auto tmpIndexList = indexer->getIndexList(query);
          for (unsigned int i : tmpIndexList) {
            if (i < _size) {
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

  unsigned int getAvailableIndex() {
    unsigned int index;
    if (_size == 0) {
      index = 0;
      _size = 1;
      _data = (DBElement<T> **)malloc(sizeof(DBElement<T> *) * _size);
    } else if (_emptyIndexList.size() > 0) {
      index = _emptyIndexList.back();
      _emptyIndexList.pop_back();
    } else {
      index = _size;
      _size += 1;
      _data = (DBElement<T> **)realloc(_data, sizeof(DBElement<T> *) * _size);
    }
    return index;
  }

  void handleModifiedCallback(DBElement<T> *element) {
    // This thread still holds the element lock
    assert(element->holdingThread() == std::this_thread::get_id() &&
           "Fatal Error! Element lock must be released after handling the "
           "modification callback.");

    for (std::shared_ptr<IIndexer<T>> indexer : _indexer) {
      try {
        indexer->valueChanged(element->value(), element->getIndex());
      } catch (const std::exception &) {
        // Ignore indexer exceptions
      }
    }
  }

public:
  Database() {}

  virtual ~Database() {
    std::lock_guard<std::mutex> guard(_lock);

    for (unsigned int i = 0; i < _size; i++) {
      if (_data[i] != nullptr) {
        _data[i]->lock();
        _data[i]->unlock();
        delete _data[i];
        _data[i] = nullptr;
      }
    }

    free(_data);
    _indexer.clear();
    _ptrMap.clear();
  }

  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  virtual void addIndexer(std::shared_ptr<IIndexer<T>> indexer) {
    std::lock_guard<std::mutex> guard(_lock);
    _indexer.push_back(indexer);
  }

  virtual int count() {
    std::lock_guard<std::mutex> guard(_lock);
    int size = _size - _emptyIndexList.size();
    assert(size == (int)_ptrMap.size() &&
           "Fatal Error! Invalid database state reached.");
    return size;
  }

  virtual DBView<T> insert(T *entry) {
    if (entry == nullptr) {
      throw DBException(ERR_ARG_NULL);
    }

    std::lock_guard<std::mutex> guard(_lock);
    if (_ptrMap.find(entry) != _ptrMap.end()) {
      std::stringstream ss;
      ss << "Object (" << entry << ") is already stored in the database."
         << std::endl;
      throw DBException(ERR_INVALID_ARG, ss.str());
    }

    unsigned int index = getAvailableIndex();

    std::pair<T *, unsigned int> ptrEntry(entry, index);
    _ptrMap.insert(ptrEntry);

    DBElement<T> *element = new DBElement<T>(entry, index);
    element->modifiedCallback = [this](DBElement<T> *elmt) {
      handleModifiedCallback(elmt);
    };
    // Assign view before adding to data to guarantee first use for creator
    DBView<T> view = element->getView();
    _data[index] = element;

    return view;
  }

  virtual std::vector<DBView<T>> get(const IQuery &query) {
    std::lock_guard<std::mutex> guard(_lock);

    auto indexList = getIndexList(query);

    std::vector<DBView<T>> views;
    for (unsigned int i : indexList) {
      views.push_back(_data[i]->getView());
    }

    return views;
  }

  virtual int remove(const IQuery &query) {
    std::lock_guard<std::mutex> guard(_lock);

    auto indexList = getIndexList(query);

    for (unsigned int i : indexList) {
      auto elmt = _data[i];
      _data[i] = nullptr;
      _emptyIndexList.push_back(i);
      _ptrMap.erase(elmt->value());
      elmt->lock();
      elmt->unlock(false);
      delete elmt;
    }

    return indexList.size();
  }

}; // namespace level

} // namespace level