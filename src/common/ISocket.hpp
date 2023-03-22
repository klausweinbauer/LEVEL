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
 * @file ISocket.hpp
 * @author Klaus Weinbauer
 * @brief Socket interface to network infrastructure
 * @version 0.1
 * @date 2022-07-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

namespace level {

/**
 * @brief Socket interface to network infrastructure
 *
 */
class ISocket {

public:
  virtual ~ISocket() {}

  /**
   * @brief Send data.
   *
   * @throw Exception For general errors.
   * @throw NetworkException For network and socket related errors.
   *
   * @param buffer Buffer containing the data to transmit.
   * @param len Length of buffer in bytes.
   * @return true Send successful.
   * @return false Send failed.
   */
  virtual bool send(const char *buffer, int len) = 0;

  /**
   * @brief Receive data. This call blocks until data is received or the timeout
   * is triggered.
   *
   * @throw Exception For general errors.
   * @throw NetworkException For network and socket related errors.
   *
   * @param buffer Buffer where received data should be stored.
   * @param len Size of the buffer.
   * @param timeout Timeout in milliseconds. If set to 0, no timeout is used.
   * @return int Returns the number of bytes stored in buffer. Returns 0 if
   * timeout is triggered.
   */
  virtual int recv(char *buffer, int len, int timeout = 0) = 0;

  /**
   * @brief Receive a fixed amount of data. This call blocks until the buffer is
   * filled completely. This operation can be canceled asynchronously by setting
   * the cancel flag.
   *
   * @throw Exception For general errors.
   * @throw NetworkException For network and socket related errors.
   *
   * @param buffer Buffer to fill with received data.
   * @param len The length of the buffer to fill.
   * @param cancel Cancel flag. Once this is set to true, the operation will
   * terminate and return to the caller.
   * @return int Returns the number of bytes received and stored in buffer. If
   * not canceled, this is equal to len.
   */
  virtual int read(char *buffer, int len,
                   const bool *const cancel = nullptr) = 0;
};

} // namespace level