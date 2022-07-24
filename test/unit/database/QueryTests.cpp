#include <Query.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

TEST(Database_Query, Test_Standard_Query) {
  int queryType = rand();
  Query q(queryType);

  ASSERT_EQ(queryType, q.getQueryType());
}