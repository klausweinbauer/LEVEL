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

namespace level::cam {

/**
 * @brief Methods for encoding and decoding messages.
 *
 */
class Encoder : public IEncoder {
private:
public:
  Encoder();
  ~Encoder();

  virtual int encode(const CAM_t *cam, uint8_t *buffer, int bufferLen,
                     EncodingType encoding = EncodingType::DER_BER) override;

  virtual CAM_t *decode(const uint8_t *buffer, int bufferLen,
                        EncodingType encoding = EncodingType::DER_BER) override;
};

} // namespace level::cam
