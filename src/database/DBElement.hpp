/**
 * @file DBElement.hpp
 * @author Klaus Weinbauer
 * @brief Manages a data object in the database
 * @version 0.1
 * @date 2022-06-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <DBView.hpp>
#include <functional>
#include <mutex>

namespace level {

template <typename TValue> class DBView;

template <typename TValue> class DBElement {
private:
  TValue *_value;
  std::mutex _lock;
  bool _modified;

public:
  std::function<void(DBElement<TValue> *)> modifiedCallback;

  DBElement(TValue *value) : _value(value), _modified(false) {}

  virtual ~DBElement() {}

  DBElement(const DBElement<TValue> &view) = delete;
  DBElement<TValue> &operator=(const DBElement<TValue> &view) = delete;

  DBView<TValue> getView() {

    DBView<TValue> view(this);
    return view;
  }

  TValue *value() { return _value; }

  bool modified() { return _modified; }

  void clearModifiedFlag() { _modified = false; }

  void unlock() { _lock.unlock(); }

  void unlock(bool accessed) {
    _modified = accessed | _modified;
    _lock.unlock();
    if (_modified && modifiedCallback) {
      modifiedCallback(this);
      _modified = false;
    }
  }

  void lock() { _lock.lock(); }

  friend class DBView<TValue>;
};

}; // namespace level