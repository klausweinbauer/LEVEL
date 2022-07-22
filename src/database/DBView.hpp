/**
 * @file DBView.hpp
 * @author Klaus Weinbauer
 * @brief Manages a transaction with a database entry
 * @version 0.1
 * @date 2022-06-28
 *
 * @copyright Copyright (c) 2022
 *
 * This class is basically a wrapper for an interaction with a database entry,
 * to ensure mutual exclusion for the caller as long as it holds the view
 * instance.
 *
 */

#pragma once

#include <DBElement.hpp>
#include <DBException.hpp>
#include <algorithm>
#include <assert.h>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// ENA_SINGLE_VIEW enables restriction for a single open view for a thread
// at any given time. Use this to prevent deadlocks caused by opening multiple
// views.
//#define ENA_SINGLE_VIEW

namespace level {

template <typename TValue> class DBElement;

template <typename TValue> class DBView {
private:
  DBElement<TValue> *_entry;
  bool _accessed;

#ifdef ENA_SINGLE_VIEW
  static std::mutex _threadListLock;
  static std::vector<std::thread::id> _threadList;

  void isAllowedToOpen() {
    std::lock_guard<std::mutex> guard(_threadListLock);

    auto id = std::this_thread::get_id();
    for (auto it = _threadList.begin(); it != _threadList.end(); it++) {
      if (id == *it) {
        throw DBException(
            ERR_INVALID_OPERATION,
            "You may only have one open DBView at a time. Opening multiple "
            "views can result in a deadlock.");
      }
    }

    _threadList.push_back(id);
  }
#endif

public:
  DBView(DBElement<TValue> *entry) : _entry(entry), _accessed(false) {
#ifdef ENA_SINGLE_VIEW
    isAllowedToOpen();
#endif
    _entry->lock();
  }

  virtual ~DBView() {
    if (_entry) {
      _entry->unlock(_accessed);
#ifdef ENA_SINGLE_VIEW
      std::lock_guard<std::mutex> guard(_threadListLock);
      auto id = std::this_thread::get_id();
      auto it = std::find(_threadList.begin(), _threadList.end(), id);

      assert(it != _threadList.end());

      _threadList.erase(it);
#endif
    }
  }

  DBView(const DBView<TValue> &view) = delete;
  DBView<TValue> &operator=(const DBView<TValue> &view) = delete;

  DBView(DBView<TValue> &&view) : _entry(nullptr), _accessed(false) {
    swap(*this, view);
  }
  DBView<TValue> &operator=(DBView<TValue> &&view) {
    if (this != std::addressof(view)) {
      swap(*this, view);
    }
    return *this;
  }

  bool accessed() { return _accessed; }

  TValue *operator->() {
    _accessed = true;
    return _entry->_value;
  }

  TValue operator*() { return *_entry->_value; }

  TValue *operator&() {
    _accessed = true;
    return _entry->_value;
  }

  friend void swap(DBView<TValue> &first, DBView<TValue> &second) {
    using std::swap;
    swap(first._accessed, second._accessed);
    swap(first._entry, second._entry);
  }
};

#ifdef ENA_SINGLE_VIEW
template <typename TValue>
std::vector<std::thread::id> DBView<TValue>::_threadList;

template <typename TValue> std::mutex DBView<TValue>::_threadListLock;

#endif

}; // namespace level