#include <Database.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

TEST(Database_Database, Test_Create) {
  Database<int> db;
  int x = rand();

  auto view = db.insert(&x);

  ASSERT_EQ(x, *view);
}

TEST(Database_Database, Test_Count_Empty_Database) {
  Database<int> db;
  ASSERT_EQ(0, db.count());
}

TEST(Database_Database, Test_Count) {
  Database<int> db;
  int x = rand();
  db.insert(&x);
  db.insert(&x);
  ASSERT_EQ(2, db.count());
}

TEST(Database_Database, Test_Insert_Nullptr) {
  Database<int> db;
  bool threwException = false;
  try {
    db.insert(nullptr);
  } catch (const DBException &) {
    threwException = true;
  }

  ASSERT_TRUE(threwException);
}
