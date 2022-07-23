#pragma once

#include <DBElement.hpp>
#include <IDatabase.hpp>
#include <IIndexer.hpp>
#include <mutex>

namespace level {

template <typename T> class Database : public IDatabase<T> {
private:
  DBElement<T> **_data = nullptr;
  unsigned int _size = 0;
  std::mutex _lock;
  std::vector<std::shared_ptr<IIndexer<T>>> _indexer;

  std::vector<unsigned int> getIndexList(const IQuery &query) {
    std::vector<unsigned int> indexList;
    for (std::shared_ptr<IIndexer<T>> indexer : _indexer) {
      if (indexer->getQueryType() == query.getQueryType()) {
        auto tmpIndexList = indexer->getIndexList(query);
        for (unsigned int i : tmpIndexList) {
          indexList.push_back(i);
        }
      }
    }

    std::sort(indexList.begin(), indexList.end());
    auto last = std::unique(indexList.begin(), indexList.end());
    indexList.erase(last, indexList.end());

    return indexList;
  }

  static void deleteEntry(DBElement<T> *entry) {
    entry->lock();
    entry->unlock(false);
    delete entry;
  }

public:
  Database() {}

  virtual ~Database() {
    std::lock_guard<std::mutex> guard(_lock);
    for (unsigned int i = 0; i < _size; i++) {
      delete _data[i];
      _data[i] = nullptr;
    }
    free(_data);

    _indexer.clear();
  }

  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  virtual void addIndexer(std::shared_ptr<IIndexer<T>> indexer) {
    std::lock_guard<std::mutex> guard(_lock);
    _indexer.push_back(indexer);
  }

  virtual int count() {
    std::lock_guard<std::mutex> guard(_lock);
    return _size;
  }

  virtual DBView<T> insert(T *entry) {
    if (entry == nullptr) {
      throw DBException(ERR_ARG_NULL);
    }

    DBElement<T> *element = new DBElement<T>(entry);
    DBView<T> view = element->getView();

    std::lock_guard<std::mutex> guard(_lock);
    if (_size == 0) {
      _size = 1;
      _data = (DBElement<T> **)malloc(sizeof(DBElement<T> *) * _size);
    } else {
      _size += 1;
      _data = (DBElement<T> **)realloc(_data, sizeof(DBElement<T> *) * _size);
    }
    _data[_size - 1] = element;

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
    /*std::lock_guard<std::mutex> guard(_lock);

    auto indexList = getIndexList(query);

    for (unsigned int i : indexList) {
      auto elmt = _data[i];
    }*/
  }

}; // namespace level

} // namespace level