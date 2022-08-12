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