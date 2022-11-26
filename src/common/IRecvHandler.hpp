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
   * @brief Add a callback function for receiving messages. The network layer
   * executes the callback when a new message is received. The receiver should
   * set the tookOwnership flag (2nd argument) to true if it took ownership of
   * the message pointer. The handler will delete the message if this flag is
   * false after all callbacks are executed.
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
   * @return int Number of registered callbacks.
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