/**
 * @file INetworkInterface.hpp
 * @author Klaus Weinbauer
 * @brief Interface for sending and receiving messages.
 * @version 0.1
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <Exception.hpp>
#include <functional>

namespace level {

/**
 * @brief Interface for sending and receiving messages.
 *
 * @tparam T Message type.
 */
template <typename T> class INetworkInterface {
public:
  virtual ~INetworkInterface() {}

  /**
   * @brief Encode and send the given message.
   *
   * @param msg Message to send.
   * @return true Message sent successful.
   * @return false Sending message failed.
   */
  virtual bool send(const T *msg) = 0;

  /**
   * @brief Callback when the network access layer received a new message.
   *
   */
  std::function<void(T *)> recvCallback;

  /**
   * @brief Callback when the network access layer encountered an error during
   * receiving.
   *
   */
  std::function<void(const Exception &)> errorCallback;
};

} // namespace level
