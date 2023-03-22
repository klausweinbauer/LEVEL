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

#include <ErrorHandler.hpp>
#include <gtest/gtest.h>

using namespace level;

TEST(ErrorHandler, AddCallback) {
  ErrorHandler handler;
  handler.registerCallback([](const Exception &e) {});
  ASSERT_EQ(1, handler.callbackCount());
}

TEST(ErrorHandler, AddMultipleCallbacks) {
  ErrorHandler handler;
  handler.registerCallback([](const Exception &e) {});
  handler.registerCallback([](const Exception &e) {});
  ASSERT_EQ(2, handler.callbackCount());
}

TEST(ErrorHandler, ClearCallbacks) {
  ErrorHandler handler;
  handler.registerCallback([](const Exception &e) {});
  handler.unregisterCallbacks();
  ASSERT_EQ(0, handler.callbackCount());
}

TEST(ErrorHandler, InvokeHandler) {
  int callCount = 0;
  int expectedErrorCode = rand();
  int actualErrorCode;
  ErrorHandler handler;
  Exception exception(expectedErrorCode);
  handler.registerCallback([&callCount, &actualErrorCode](const Exception &e) {
    callCount++;
    actualErrorCode = e.getErrCode();
  });
  handler.invoke(exception);
  ASSERT_EQ(1, callCount);
  ASSERT_EQ(expectedErrorCode, actualErrorCode);
}