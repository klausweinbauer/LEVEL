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
   * @return true Send successful.
   * @return false Send failed.
   */
  virtual bool send(const char *buffer, int len) = 0;

  /**
   * @brief Receive data. This call blocks until data is received or the timeout
   * is triggered.
   *
   * @param buffer Buffer where received data should be stored.
   * @param len Size of the buffer.
   * @param timeout Timeout in milliseconds. If set to 0, no timeout is used.
   * @return int Returns the number of bytes stored in buffer.
   */
  virtual int recv(char *buffer, int len, int timeout = 0) = 0;

  /**
   * @brief Receive a fixed amount of data. This call blocks until the buffer is
   * filled completely. This operation can be canceled asynchronously by setting
   * the cancel flag.
   *
   * @param buffer Buffer to fill with received data.
   * @param len The length of the buffer to fill.
   * @param cancel Cancel flag. Once this is set to true, the operation will
   * terminate and return to the caller.
   * @return true The buffer is filled with received data.
   * @return false Operation failed or was canceled.
   */
  virtual bool read(char *buffer, int len, bool *cancel = nullptr) = 0;
};

} // namespace level