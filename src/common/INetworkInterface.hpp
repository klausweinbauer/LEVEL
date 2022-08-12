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
#include <IErrorHandler.hpp>
#include <IRecvHandler.hpp>
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
   * @brief Get a pointer to the receive handler.
   *
   * @return std::shared_ptr<IRecvHandler<T>> Receive handler instance.
   */
  std::shared_ptr<IRecvHandler<T>> getRecvHandler();

  /**
   * @brief Get a pointer to the error handler.
   *
   * @return std::shared_ptr<IErrorHandler> Error handler instance.
   */
  std::shared_ptr<IErrorHandler> getErrorHandler();
};

} // namespace level
