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
#include <mutex>

template <typename TID, typename TValue> class DBView;

template <typename TID, typename TValue> class DBElement {
private:
  TValue *_value;
  std::mutex _lock;

public:
  const TID id;

  DBElement(TID id, TValue *value) : id(id), _value(value) {}

  virtual ~DBElement() { delete _value; }

  DBElement(const DBElement<TID, TValue> &view) = delete;
  DBElement<TID, TValue> &
  operator=(const DBElement<TID, TValue> &view) = delete;

  DBView<TID, TValue> getView() {

    DBView<TID, TValue> view(this);
    return view;
  }

  friend class DBView<TID, TValue>;
};