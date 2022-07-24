#include <Database.hpp>
#include <Mocks.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

TEST(Database, Heavy_Querying) {
  Database<int> db;
  ASSERT_EQ(0, db.count());
}