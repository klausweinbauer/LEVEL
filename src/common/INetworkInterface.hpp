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
  virtual std::shared_ptr<IRecvHandler<T>> getRecvHandler() = 0;

  /**
   * @brief Get a pointer to the error handler.
   *
   * @return std::shared_ptr<IErrorHandler> Error handler instance.
   */
  virtual std::shared_ptr<IErrorHandler> getErrorHandler() = 0;
};

} // namespace level
