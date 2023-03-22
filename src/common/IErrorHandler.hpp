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