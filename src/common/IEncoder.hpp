/**
 * @file IEncoder.hpp
 * @author Klaus Weinbauer
 * @brief Message encoder interface.
 * @version 0.1
 * @date 2022-07-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <Types.hpp>
#include <memory>
#include <vector>

namespace level {

/**
 * @brief Message encoder interface.
 *
 * @tparam T Message type.
 */
template <typename T> class IEncoder {

public:
  virtual ~IEncoder() {}

  /**
   * @brief Encode a message.
   *
   * @param message Message to encode.
   * @return int Returns the length of the encoded message.
   */

  /**
   * @brief Encodes a message.
   *
   * @throw EncodeException if encountered an error during encoding.
   *
   * @param message Message to encode.
   * @return std::vector<BYTE> Returns the encoded message.
   */
  virtual std::vector<BYTE> encode(const std::shared_ptr<T> message) = 0;

  /**
   * @brief Decode a message.
   *
   * @throw EncodeException if encountered an error during decoding.
   *
   * @param buffer Buffer that contains the encoded message.
   * @return std::shared_ptr<T> Returns a newly allocated message.
   */
  virtual std::shared_ptr<T> decode(const std::vector<BYTE> buffer) = 0;

  /**
   * @brief Decode a message.
   *
   * @param buffer Buffer that contains the encoded message.
   * @param bufferLen Length of the encoded message.
   * @return std::shared_ptr<T> Returns a newly allocated message.
   */
  virtual std::shared_ptr<T> decode(const BYTE *buffer, int bufferLen) = 0;
};

} // namespace level
