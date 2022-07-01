/**
 * @file INetworkInterface.hpp
 * @author Klaus Weinbauer
 * @brief Interface for sending and receiving CAMs.
 * @version 0.1
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <CAM.h>
#include <c2xcommon.h>

namespace c2x::cam {

/**
 * @brief Interface for sending and receiving CAMs.
 *
 */
class INetworkInterface {
public:
  virtual ~INetworkInterface(){};

  /**
   * @brief @details Use \ref EncodingType "XER" encoding to achieve better
   * human readability and \ref EncodingType "DER_BER" for a compact encoding,
   * lower bandwidth usage, and higher performance.
   *
   * @param encoding Type of encoding.
   */
  virtual void setEncoding(EncodingType encoding) = 0;

  /**
   * @brief Encode and send the given message.
   *
   * @param cam Message to send.
   */
  virtual void send(const CAM_t *cam) = 0;

  /**
   * @brief Callback when the network access layer received a new message.
   *
   */
  std::function<void(CAM_t *)> recvCallback;

  /**
   * @brief Callback when the network access layer encountered an error during
   * receiving.
   *
   */
  std::function<void(const Exception &)> recvFailedCallback;
};

} // namespace c2x::cam
