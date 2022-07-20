#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <level.h>
#include <string.h>

TEST(API, Test_Get_Version) {
  auto version = level::getVersion();
  ASSERT_TRUE(0 == strcmp("0.1.0", version));
}