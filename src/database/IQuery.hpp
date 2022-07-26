/**
 * @file IQuery.hpp
 * @author Klaus Weinbauer
 * @brief Query interface used to implement database logic requests.
 * @version 0.1
 * @date 2022-07-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

namespace level {

/**
 * @brief Qquery interface used to implement database logic requests.
 *
 */
class IQuery {
public:
  virtual ~IQuery(){};
};

} // namespace level