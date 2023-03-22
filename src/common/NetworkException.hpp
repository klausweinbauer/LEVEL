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
 * @file NetworkException.hpp
 * @author Klaus Weinbauer
 * @brief General exception for errors while sending or receiving messages.
 * @version 0.1
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <Exception.hpp>

namespace level {

/**
 * @brief General exception for errors while sending or receiving messages.
 *
 */
class NetworkException : public Exception {

public:
  NetworkException() : Exception() {}
  NetworkException(int errCode)
      : Exception(errCode, "General network exception.") {}
  NetworkException(int errCode, std::string errMsg)
      : Exception(errCode, errMsg) {}
  ~NetworkException() {}
};

} // namespace level
