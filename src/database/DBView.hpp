/**
 * @file DBView.hpp
 * @author Klaus Weinbauer
 * @brief Manages a transaction with a database element
 * @version 0.1
 * @date 2022-06-28
 *
 * @copyright Copyright (c) 2022
 *
 * This class is basically a wrapper for an interaction with a database element,
 * to ensure mutual exclusion for the caller as long as it holds the view
 * instance.
 *
 */

#pragma once

#include <DBElement.hpp>
#include <DBElementStatus.hpp>
#include <DBException.hpp>
#include <algorithm>
#include <assert.h>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace level {

template <typename T> class DBElement;

template <typename T> class DBView {
private:
  DBElement<T> *_element;
  DBElementStatus *_status;

public:
  /**
   * @brief Construct a new DBView object
   *
   * @details Construction is blocked until the element is safely acquired. Once
   * this object goes out of scope, it releases the locked element.
   *
   * @param element Element to acquire.
   * @param status Reference to the database status for this element and view.
   * After successful construction, the view owns this resource.
   */
  DBView(DBElement<T> *element, DBElementStatus *status)
      : _element(element), _status(status) {
    if (element == nullptr) {
      throw Exception(ERR, "Parameter 'element' may not be null.");
    }
    if (status == nullptr) {
      throw Exception(ERR, "Parameter 'status' may not be null.");
    }
    _element->lock();
  }

  virtual ~DBView() {
    if (_element == nullptr) {
      return;
    }

    if (!_status->_deleted) {
      _element->unlock();
    }
    delete _status;
  }

  DBView(const DBView<T> &view) = delete;
  DBView<T> &operator=(const DBView<T> &view) = delete;

  DBView(DBView<T> &&view) : _element(nullptr), _status(nullptr) {
    swap(*this, view);
  }
  DBView<T> &operator=(DBView<T> &&view) {
    if (this != std::addressof(view)) {
      swap(*this, view);
    }
    return *this;
  }

  T &operator->() {}

  T &operator*() {}

  friend void swap(DBView<T> &first, DBView<T> &second) {
    using std::swap;
    swap(first._status, second._status);
    swap(first._element, second._element);
  }
};

}; // namespace level