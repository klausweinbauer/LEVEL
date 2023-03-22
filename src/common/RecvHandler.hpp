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
 * @file RecvHandler.hpp
 * @author Klaus Weinbauer
 * @brief Implementation for IRecvHandler interface.
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <IRecvHandler.hpp>
#include <vector>

namespace level {

/**
 * @brief Implementation for IRecvHandler interface.
 *
 * @tparam T Message type.
 */
template <typename T> class RecvHandler : public IRecvHandler<T> {
protected:
  std::vector<std::function<void(T *msg, bool *tookOwnership)>> _callbacks;

  virtual void freeMessage(T *msg) = 0;

public:
  RecvHandler() : _callbacks() {}

  virtual ~RecvHandler() {}

  virtual void registerCallback(
      std::function<void(T *msg, bool *tookOwnership)> callback) override {
    _callbacks.push_back(callback);
  }

  virtual void unregisterCallbacks() override { _callbacks.clear(); }

  virtual int callbackCount() override { return _callbacks.size(); }

  virtual void invoke(T *msg) override {
    bool tookOwnership = false;
    for (auto &&callback : _callbacks) {
      callback(msg, &tookOwnership);
    }
    if (!tookOwnership) {
      this->freeMessage(msg);
    }
  }
};

} // namespace level