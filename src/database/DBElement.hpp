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
  T *_data;
  const std::mutex _lock;
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

  virtual DBView<T> getView() {}

  virtual T &data() const {}
  virtual void setData(T *data) {}

  virtual void unlock() {}
  virtual void lock() {}

  virtual void clear() {}

  virtual const std::thread::id &holdingThread() const {}

  virtual unsigned int getIndex() const {}
};

}; // namespace level