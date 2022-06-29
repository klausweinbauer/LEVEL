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
#include <iostream>
#include <mutex>
#include <string>

template <typename TID, typename TValue> class DBElement;

template <typename TID, typename TValue> class DBView {
private:
  DBElement<TID, TValue> *_entry;

public:
  DBView(DBElement<TID, TValue> *entry) : _entry(entry) {
    _entry->_lock.lock();
  }

  virtual ~DBView() { _entry->_lock.unlock(); }

  DBView(const DBView<TID, TValue> &view) = delete;
  DBView<TID, TValue> &operator=(const DBView<TID, TValue> &view) = delete;

  DBView(DBView<TID, TValue> &&view) : _entry(nullptr) {
    _entry = view._entry;
    view._entry = nullptr;
  }
  DBView<TID, TValue> &operator=(DBView<TID, TValue> &&view) {
    if (this != &view) {
      _entry = view._entry;
      view._entry = nullptr;
    }
  }

  TValue *operator->() { return _entry->_value; }
  TID id() { return _entry->id; }
};