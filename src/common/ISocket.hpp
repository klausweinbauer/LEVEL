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
   * @param buffer Buffer containing the data to transmit.
   * @param len Length of buffer in bytes.
   */
  virtual void send(const char *buffer, int len) = 0;

  /**
   * @brief Receive data.
   *
   * @param buffer Buffer where received data should be stored.
   * @param len Size of the buffer.
   * @return int Returns the number of bytes stored in buffer.
   */
  virtual int recv(char *buffer, int len) = 0;

  /**
   * @brief Close the socket.
   *
   */
  virtual void close() = 0;
};

} // namespace level