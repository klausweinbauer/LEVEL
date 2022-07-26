/**
 * @file DBException.hpp
 * @author Klaus Weinbauer
 * @brief General exception for errors during database operations.
 * @version 0.1
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <Exception.hpp>
#include <exception>
#include <sstream>
#include <string>

namespace level {

/**
 * @brief General exception for errors during database operations.
 *
 */
class DBException : public Exception {

public:
  DBException(int errCode)
      : Exception(errCode, "General database exception."){};
  DBException(int errCode, std::string errMsg) : Exception(errCode, errMsg){};
};

} // namespace level
