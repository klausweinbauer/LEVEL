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
  std::thread::id _threadId;
  unsigned int _index;

public:
  std::function<void(DBElement<TValue> *)> modifiedCallback;

  DBElement(TValue *value, unsigned int index = 0)
      : _value(value), _modified(false), _index(index) {}

  virtual ~DBElement() { delete _value; }

  DBElement(const DBElement<TValue> &view) = delete;
  DBElement<TValue> &operator=(const DBElement<TValue> &view) = delete;

  DBView<TValue> getView() {

    DBView<TValue> view(this);
    return view;
  }

  TValue *value() { return _value; }

  bool modified() { return _modified; }

  void setIndex(unsigned int index) { _index = index; }

  unsigned int getIndex() { return _index; }

  void clearModifiedFlag() { _modified = false; }

  const std::thread::id &holdingThread() const { return _threadId; }

  void unlock() { _lock.unlock(); }

  void unlock(bool accessed) {
    _modified = accessed | _modified;
    if (_modified && modifiedCallback) {
      modifiedCallback(this);
      _modified = false;
    }
    _threadId = std::thread::id();
    _lock.unlock();
  }

  void lock() {
    if (_threadId == std::this_thread::get_id()) {
      std::string msg("This thread already owns the lock for this database "
                      "entry. You may only open one view per entry.");
      throw DBException(ERR_INVALID_OPERATION, msg);
    }

    _lock.lock();
    _threadId = std::this_thread::get_id();
  }

  friend class DBView<TValue>;
};

}; // namespace level