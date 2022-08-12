/**
 * @file DBElement.hpp
 * @author Klaus Weinbauer
 * @brief Container class for a data entry in the database. This class manages a
 * data object within the database.
 * @version 0.1
 * @date 2022-06-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <DBException.hpp>
#include <DBView.hpp>
#include <IDatabase.hpp>
#include <functional>
#include <mutex>
#include <thread>

namespace level {

/**
 * @brief Container class for a data entry in the database. This class manages a
 * data object within the database.
 *
 * @tparam T Type of data to store in database. This type must implement a copy
 * constructor.
 */
template <typename T> class DBElement {
private:
  std::unique_ptr<T> _data;
  std::mutex _lock;
  std::thread::id _threadId;
  const unsigned int _index;

public:
  DBElement() : DBElement<T>(0) {}

  DBElement(unsigned int index)
      : _data(nullptr), _threadId(std::thread::id()), _index(index) {}

  virtual ~DBElement() {}

  DBElement(const DBElement<T> &element) = delete;
  DBElement<T> &operator=(const DBElement<T> &element) = delete;

  DBElement(DBElement<T> &&element) = delete;
  DBElement<T> &operator=(DBElement<T> &&element) = delete;

  /**
   * @brief Callback method to handle data update.
   *
   */
  std::function<void(const DBElement<T> *const)> updateCallback;

  /**
   * @brief Callback method to handle data deletion.
   *
   */
  std::function<void(DBElement<T> *)> removeCallback;

  /**
   * @brief Get the Data object.
   *
   * @throw DBException when no data is set.
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
   * @brief Checks if the element has a data object assigned.
   *
   * @return true The element has a data object.
   * @return false The elements' data is null.
   */
  virtual bool hasData() { return _data != nullptr; }

  /**
   * @brief Unlock the database element.
   *
   */
  virtual void unlock() {
    if (_data) {
      // Invalidation of possibly stored pointers by the user.
      _data = std::make_unique<T>(*_data);
      if (updateCallback) {
        updateCallback(this);
      }
    }

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
   * @brief Delete data associated with this element and call database to remove
   * element. DO NOT USE THIS METHOD IN DATABASE.
   *
   */
  virtual void clear() {
    if (removeCallback) {
      removeCallback(this);
    }
    _data = nullptr;
  }

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

} // namespace level