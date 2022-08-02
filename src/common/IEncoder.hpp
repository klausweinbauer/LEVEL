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
   * @brief Encodes a message.
   *
   * @throw EncodeException if encountered an error during encoding.
   *
   * @param message Message to encode.
   * @return std::vector<char> Returns the encoded message.
   */
  virtual std::vector<char> encode(const T *message) = 0;

  /**
   * @brief Decode a message.
   *
   * @throw EncodeException if encountered an error during decoding.
   *
   * @param buffer Buffer that contains the encoded message.
   * @return T* Returns a newly allocated message.
   */
  virtual T *decode(const std::vector<char> buffer) = 0;

  /**
   * @brief Decode a message.
   *
   * @throw EncodeException if encountered an error during decoding.
   *
   * @param buffer Buffer that contains the encoded message.
   * @param bufferLen Length of the encoded message.
   * @return T* Returns a newly allocated message.
   */
  virtual T *decode(const char *buffer, int bufferLen) = 0;
};

} // namespace level
