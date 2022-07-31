#include <Types.hpp>
#include <gtest/gtest.h>

using namespace level;

TEST(Type, GetDefaultTypeName) {
  ASSERT_EQ(typeid(int).name(), TypeName<int>::get());
}