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
  std::shared_ptr<DBElement<T>> _element;

  void guaranteeValidity() const {
    if (!isValid()) {
      throw DBException(ERR_INVALID_OPERATION, "DBView is not valid anymore.");
    }
  }

public:
  /**
   * @brief Construct a new DBView object
   *
   * @details Construction is blocked until the element is safely acquired. Once
   * this object goes out of scope, it releases the locked element. As long as
   * this object is valid, the data can be accessed by * or -> operators.
   *
   * @param element Element to acquire.
   */
  DBView(std::shared_ptr<DBElement<T>> element) : _element(element) {
    if (!element) {
      throw Exception(ERR, "Parameter 'element' may not be null.");
    }

    _element->lock();
  }

  virtual ~DBView() {
    if (!_element) {
      return;
    }

    _element->unlock();
  }

  DBView(const DBView<T> &view) = delete;

  DBView<T> &operator=(const DBView<T> &view) = delete;

  DBView(DBView<T> &&view) : _element(nullptr) { swap(*this, view); }

  DBView<T> &operator=(DBView<T> &&view) {
    if (this != std::addressof(view)) {
      swap(*this, view);
    }
    return *this;
  }

  T *operator->() {
    guaranteeValidity();
    return &_element->data();
  }

  T &operator*() {
    guaranteeValidity();
    return _element->data();
  }

  /**
   * @brief Delete the corresponding element in the database.
   *
   */
  void remove() {
    if (!_element) {
      return;
    }
    _element->clear();
    _element = nullptr;
  }

  /**
   * @brief Check if this view is still valid.
   *
   * @details For example, the view is no longer valid if the element has been
   * removed.
   *
   * @return true If the view is valid and the database element can be accessed.
   * @return false You are not allowed to access the database element any more.
   */
  bool isValid() const { return _element != nullptr; }

  /**
   * @brief Get the index of the database element.
   *
   * @return unsigned int Element index.
   */
  unsigned int index() const {
    guaranteeValidity();
    return _element->index();
  }

  friend void swap(DBView<T> &first, DBView<T> &second) {
    using std::swap;
    swap(first._element, second._element);
  }
};

}; // namespace level