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

#pragma once

#include <IErrorHandler.hpp>

namespace level {

class ErrorHandler : public IErrorHandler {
protected:
  std::vector<std::function<void(const Exception &)>> _callbacks;

public:
  virtual ~ErrorHandler() {}

  virtual void
  registerCallback(std::function<void(const Exception &)> callback) override {
    _callbacks.push_back(callback);
  }

  virtual void unregisterCallbacks() override { _callbacks.clear(); }

  virtual int callbackCount() override { return _callbacks.size(); }

  virtual void invoke(const Exception &exception) override {
    for (auto &&callback : _callbacks) {
      callback(exception);
    }
  }
};

} // namespace level