/**
 * @file IEncoder.hpp
 * @author Klaus Weinbauer
 * @brief Message encoder interface.
 * @version 0.1
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <CAM.h>
#include <level_common.h>

namespace level {
namespace cam {

/**
 * @brief Message encoder interface.
 *
 */
class IEncoder {

public:
  virtual ~IEncoder(){};

  /**
   * @brief Encode a CAM.
   *
   * @param cam Message to encode.
   * @param buffer Buffer where the encoded message is to be written.
   * @param bufferLen Length of the provided buffer.
   * @return int Returns the length of the encoded message.
   */
  virtual int encode(const CAM_t *cam, uint8_t *buffer, int bufferLen) = 0;

  /**
   * @brief Decode a CAM.
   *
   * @param buffer Buffer that contains the encoded message.
   * @param bufferLen Length of the encoded message.
   * @return CAM_t* Returns a newly allocated CAM.
   */
  virtual CAM_t *decode(const uint8_t *buffer, int bufferLen) = 0;
};

} // namespace cam
} // namespace level
