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
#include <functional>
#include <level_common.h>

namespace level {
namespace cam {

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

} // namespace cam
} // namespace level
