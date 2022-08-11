/**
 * @file IErrorHandler.hpp
 * @author Klaus Weinbauer
 * @brief Interface to handle error callbacks from network layer.
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <Exception.hpp>
#include <functional>

namespace level {

/**
 * @brief Interface to handle error callbacks from network layer.
 */
class IErrorHandler {
public:
  virtual ~IErrorHandler() {}

  virtual void
  registerCallback(std::function<void(const Exception &)> callback) = 0;

  virtual void unregisterCallbacks() = 0;

  virtual int callbackCount() = 0;

  virtual void invoke(const Exception &exception) = 0;
};

} // namespace level