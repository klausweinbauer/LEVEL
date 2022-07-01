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

namespace c2x {

/**
 * @brief General exception for errors while sending or receiving messages.
 *
 */
class NetworkException : Exception {

public:
  NetworkException(int errCode)
      : Exception(errCode, "General network exception."){};
  NetworkException(int errCode, std::string errMsg)
      : Exception(errCode, errMsg){};
  ~NetworkException();
};

} // namespace c2x
