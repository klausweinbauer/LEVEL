#include <DataExtensionMethods.hpp>
#include <Exception.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

TEST(DataExtensionMethod, GetSuccessful) {
  int *src = new int;
  int *prop = get<int>(&src);
  ASSERT_EQ(src, prop);
  delete src;
}

TEST(DataExtensionMethod, GetUninitialized) {
  int *src = nullptr;
  int *prop = get<int>(&src);
  ASSERT_NE(nullptr, src);
  ASSERT_EQ(src, prop);
  free(src);
}

TEST(DataExtensionMethod, GetThrowsOnNullptrArg) {
  ASSERT_THROW(get<int>(nullptr), Exception);
}