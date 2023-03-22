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

#include <CAMRecvHandler.hpp>
#include <DENMRecvHandler.hpp>
#include <RecvHandler.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace level::RecvHandlerTests {

class IntRecvHandler : public RecvHandler<int> {
protected:
  void freeMessage(int *msg) override {
    free(msg);
    freeCalled = true;
  }

public:
  bool freeCalled = false;
};

} // namespace level::RecvHandlerTests

using namespace level;
using namespace level::RecvHandlerTests;

TEST(RecvHandler, AddCallback) {
  IntRecvHandler handler;
  handler.registerCallback([](int *msg, bool *tookOwnership) {});
  ASSERT_EQ(1, handler.callbackCount());
}

TEST(RecvHandler, AddMultipleCallbacks) {
  IntRecvHandler handler;
  handler.registerCallback([](int *msg, bool *tookOwnership) {});
  handler.registerCallback([](int *msg, bool *tookOwnership) {});
  ASSERT_EQ(2, handler.callbackCount());
}

TEST(RecvHandler, ClearCallbacks) {
  IntRecvHandler handler;
  handler.registerCallback([](int *msg, bool *tookOwnership) {});
  handler.unregisterCallbacks();
  ASSERT_EQ(0, handler.callbackCount());
}

TEST(RecvHandler, InvokeHandler) {
  int callCount = 0;
  int *expectedMsgPtr = (int *)calloc(1, sizeof(int));
  *expectedMsgPtr = rand();
  int expectedMsg = *expectedMsgPtr;
  int callbackMsg = 0;
  IntRecvHandler handler;
  auto callback = [&callCount, &callbackMsg](int *msg, bool *tookOwnership) {
    callCount++;
    callbackMsg = *msg;
  };
  handler.registerCallback(callback);
  handler.invoke(expectedMsgPtr);
  ASSERT_TRUE(handler.freeCalled);
  ASSERT_EQ(expectedMsg, callbackMsg);
  ASSERT_EQ(1, callCount);
}

TEST(RecvHandler, TakeOwnershipInCallback) {
  int *msgPtr = (int *)calloc(1, sizeof(int));
  IntRecvHandler handler;
  handler.registerCallback(
      [](int *msg, bool *tookOwnership) { *tookOwnership = true; });
  handler.invoke(msgPtr);
  ASSERT_FALSE(handler.freeCalled);
  free(msgPtr);
}

TEST(RecvHandler, HasOwnershipInSecondCallback) {
  int *msgPtr = (int *)calloc(1, sizeof(int));
  bool hasOwnershipInSecondCallback = false;
  IntRecvHandler handler;
  handler.registerCallback(
      [](int *msg, bool *tookOwnership) { *tookOwnership = true; });
  handler.registerCallback(
      [&hasOwnershipInSecondCallback](int *msg, bool *tookOwnership) {
        hasOwnershipInSecondCallback = *tookOwnership;
      });
  handler.invoke(msgPtr);
  ASSERT_TRUE(hasOwnershipInSecondCallback);
  free(msgPtr);
}

TEST(RecvHandler, CAMReceiveHandlerFreeMessage) {
  using namespace level::cam;
  CAM *cam = (CAM *)calloc(1, sizeof(CAM));
  CAMRecvHandler handler;
  handler.invoke(cam);
  // Expect no memory leak
}

TEST(RecvHandler, DENMReceiveHandlerFreeMessage) {
  using namespace level::denm;
  DENM *denm = (DENM *)calloc(1, sizeof(DENM));
  DENMRecvHandler handler;
  handler.invoke(denm);
  // Expect no memory leak
}