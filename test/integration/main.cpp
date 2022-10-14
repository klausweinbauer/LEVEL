#include <gtest/gtest.h>
#include <level.h>
#include <string.h>
#include <time.h>

using namespace level;

int main(int argc, char **argv) {
  srand(time(0));

  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  clearErrMsg();
  return ret;
}