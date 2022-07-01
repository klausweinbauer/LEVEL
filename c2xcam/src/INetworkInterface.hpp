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
   * @brief Encode and send the given message.
   *
   * @details Use \ref EncodingType "XER" encoding to achieve better human
   * readability and \ref EncodingType "DER_BER" for a compact encoding, lower
   * bandwidth usage, and higher performance.
   *
   * @param cam Message to send.
   * @param encoding Type of message encoding.
   */
  virtual void send(const CAM_t *cam,
                    EncodingType encoding = EncodingType::DER_BER) = 0;

  /**
   * @brief Callback when the network access layer received a new message.
   *
   */
  std::function<void(CAM_t *)> recvCallback;
};

} // namespace c2x::cam
