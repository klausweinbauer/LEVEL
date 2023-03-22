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
 * @file EncodeException.hpp
 * @author Klaus Weinbauer
 * @brief General exception for errors while encoding or decoding messages.
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <Exception.hpp>
#include <string>

namespace level {

/**
 * @brief General exception for errors while encoding or decoding messages.
 *
 */
class EncodeException : public Exception {
public:
  EncodeException() : Exception(ERR_ENCODE, "Message de-/encoding failed.") {}

  EncodeException(std::string encodingType, std::string messageType) {
    std::stringstream stream;
    stream << "Message de-/encoding failed. (Encoding: " << encodingType
           << ", Message: " << messageType << ")" << std::endl;
    Exception(ERR_ENCODE, stream.str());
  }

  EncodeException(std::string encodingType, std::string messageType,
                  std::string messageProperty, std::string errorType) {
    std::stringstream stream;
    stream << encodingType << " encoding failed for message " << messageType
           << " with error type '" << errorType
           << "'. This is probably due to an invalid value of property '"
           << messageProperty << "'." << std::endl;
    Exception(ERR_ENCODE, stream.str());
  }
};

} // namespace level