#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <level.h>
#include <string.h>
#include <time.h>

using namespace level;

int main(int argc, char **argv) {
  srand(time(0));

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);
  int ret = RUN_ALL_TESTS();
  clearLastErrMsg();
  return ret;
}