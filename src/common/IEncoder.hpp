// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

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
