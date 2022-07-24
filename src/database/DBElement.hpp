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
#include <thread>

namespace level {

template <typename T> class DBElement {
private:
  std::unique_ptr<T> _data;
  std::mutex _lock;
  std::thread::id _threadId;
  const unsigned int _index;

public:
  DBElement(unsigned int index)
      : _data(nullptr), _threadId(std::thread::id()), _index(index) {}

  virtual ~DBElement() {}

  DBElement(const DBElement<T> &element) = delete;
  DBElement<T> &operator=(const DBElement<T> &element) = delete;

  DBElement(DBElement<T> &&element) = delete;
  DBElement<T> &operator=(DBElement<T> &&element) = delete;

  /**
   * @brief Get the Data object.
   *
   * @return T& Data object.
   */
  virtual T &data() const {
    if (!_data) {
      throw DBException(ERR_NULL, "No data set for this element.");
    }
    return *_data;
  }

  /**
   * @brief Set the Data object.
   *
   * @param data
   */
  virtual void setData(std::unique_ptr<T> data) { _data = std::move(data); }

  /**
   * @brief Unlock the database element.
   *
   */
  virtual void unlock() {
    _threadId = std::thread::id();
    _lock.unlock();
  }

  /**
   * @brief Lock the database element. This call blocks until the element is
   * safely acquired.
   *
   */
  virtual void lock() {
    _lock.lock();
    _threadId = std::this_thread::get_id();
  }

  /**
   * @brief Delete data associated with this element.
   *
   */
  virtual void clear() { _data = nullptr; }

  /**
   * @brief Returns the id for the thread which currently holds the lock. If the
   * calling thread is different from the holding thread, the result is not
   * guaranteed valid.
   *
   * @return const std::thread::id& Thread, which probably holds the lock.
   */
  virtual const std::thread::id &holdingThread() const { return _threadId; }

  /**
   * @brief Method to get the index in the database.
   *
   * @return unsigned int Index in database.
   */
  virtual unsigned int index() const { return _index; }
};

}; // namespace level