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