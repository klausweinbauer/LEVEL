// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

/**
 * @file DBView.hpp
 * @author Klaus Weinbauer
 * @brief Data access container for database entries. This class manages a
 * transaction with a database element to ensure thread- and memory-safe access
 * for the caller as long as it holds the view instance.
 * @version 0.1
 * @date 2022-06-28
 *
 * @copyright Copyright (c) 2022
 *
 * @details
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

/**
 * @brief Data access container for database entries. This class manages a
 * transaction with a database element to ensure thread- and memory-safe access
 * for the caller as long as it holds the view instance.
 *
 * @tparam T Type of data to store in database.
 */
template <typename T> class DBView {
private:
  DBElement<T> *_element;

  /**
   * @brief Throws exception if state is invalid.
   *
   * @throw DBException when state is invalid.
   */
  void guaranteeValidity() const {
    if (!isValid()) {
      throw DBException(ERR_INVALID_OPERATION, "DBView is not valid anymore.");
    }
  }

public:
  /**
   * @brief Construct a new DBView object. Construction is blocked until the
   * element is safely acquired. Once this object goes out of scope, it releases
   * the locked element. As long as this object is valid, the data can be
   * accessed by * or -> operators.
   *
   * @throw Exception when argument element is null.
   *
   * @param element Element to acquire.
   */
  DBView(DBElement<T> *element) : _element(element) {
    if (!element) {
      throw Exception(ERR_ARG_NULL, "Parameter 'element' may not be null.");
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
    swap(*this, view);
    return *this;
  }

  /**
   * @brief Data access operator.
   *
   * @throw DBException when state is invalid.
   *
   * @return T* Data object.
   */
  T *operator->() {
    guaranteeValidity();
    return &_element->data();
  }

  /**
   * @brief Data access operator.
   *
   * @throw DBException when state is invalid.
   *
   * @return T&
   */
  T &operator*() {
    guaranteeValidity();
    return _element->data();
  }

  /**
   * @brief Delete the corresponding element in the database.
   *
   */
  bool remove() {
    if (!_element) {
      return false;
    }
    _element->clear();
    _element->unlock();
    _element = nullptr;
    return true;
  }

  /**
   * @brief Check if this view is still valid. For example, the view is no
   * longer valid if the element has been removed.
   *
   * @return true If the view is valid and the database element can be accessed.
   * @return false You are not allowed to access the database element any more.
   */
  bool isValid() const { return _element != nullptr; }

  /**
   * @brief Get the index of the database element.
   *
   * @throw DBException when state is invalid.
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

} // namespace level