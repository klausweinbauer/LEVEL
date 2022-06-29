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

template <typename TValue> class DBElement;

template <typename TValue> class DBView {
private:
  DBElement<TValue> *_entry;
  bool _accessed;

public:
  DBView(DBElement<TValue> *entry) : _entry(entry), _accessed(false) {
    _entry->lock();
  }

  virtual ~DBView() { _entry->unlock(_accessed); }

  DBView(const DBView<TValue> &view) = delete;
  DBView<TValue> &operator=(const DBView<TValue> &view) = delete;

  DBView(DBView<TValue> &&view) : _entry(nullptr) {
    _entry = view._entry;
    view._entry = nullptr;
  }
  DBView<TValue> &operator=(DBView<TValue> &&view) {
    if (this != &view) {
      _entry = view._entry;
      view._entry = nullptr;
    }
  }

  TValue *operator->() {
    _accessed = true;
    return _entry->_value;
  }
};