#pragma once

#include <DBElement.hpp>
#include <IDatabase.hpp>

namespace level {

template <typename T> class Database : public IDatabase<T> {
private:
  std::vector<DBElement<T> *> _entries;

public:
  virtual ~Database() {
    for (auto it = _entries.begin(); it != _entries.end(); it++) {
      delete *it;
    }
    _entries.clear();
  }

  virtual int count() { return _entries.size(); }

  virtual DBView<T> insert(T *entry) {
    if (entry == nullptr) {
      throw DBException(ERR_ARG_NULL);
    }

    DBElement<T> *element = new DBElement<T>(entry);
    DBView<T> view = element->getView();
    _entries.push_back(element);
    return view;
  }

  virtual std::vector<DBView<T>> get(IQuery query) {}

  virtual int remove(IQuery query) {}
};

} // namespace level