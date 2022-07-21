/**
 * @file Encoder.hpp
 * @author Klaus Weinbauer
 * @brief Methods for encoding and decoding messages.
 * @version 0.1
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <IEncoder.hpp>

namespace level {
namespace cam {

/**
 * @brief Methods for encoding and decoding messages.
 *
 */
class Encoder : public IEncoder {
private:
  EncodingType _encoding;

public:
  Encoder(EncodingType encoding = EncodingType::DER_BER);
  ~Encoder();

  virtual int encode(const CAM_t *cam, uint8_t *buffer, int bufferLen) override;

  virtual CAM_t *decode(const uint8_t *buffer, int bufferLen) override;
};

} // namespace cam
} // namespace level
