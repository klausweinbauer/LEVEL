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
 * @file IRecvHandler.hpp
 * @author Klaus Weinbauer
 * @brief Interface to handle receive callbacks from network layer.
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <functional>

namespace level {

/**
 * @brief Interface to handle receive callbacks from network layer.
 *
 * @tparam T Message type.
 */
template <typename T> class IRecvHandler {
public:
  virtual ~IRecvHandler() {}

  /**
   * @brief Add a callback function called by the network layer when a new
   * message is received. The callback message should set the tookOwnership flag
   * (2nd argument) to true if it took ownership of the message pointer. The
   * handler will delete the message if this flag is false after all callbacks
   * are executed.
   *
   * @param callback Callback function.
   */
  virtual void registerCallback(
      std::function<void(T *msg, bool *tookOwnership)> callback) = 0;

  /**
   * @brief Remove all callback functions.
   *
   */
  virtual void unregisterCallbacks() = 0;

  /**
   * @brief Get the number of registered callbacks.
   *
   * @return int Number of callbacks.
   */
  virtual int callbackCount() = 0;

  /**
   * @brief Method to invoke the handler and execute the callbacks.
   *
   * @param msg Message object.
   */
  virtual void invoke(T *msg) = 0;
};

} // namespace level