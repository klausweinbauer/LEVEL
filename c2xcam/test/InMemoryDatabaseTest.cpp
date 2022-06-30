#include <DBException.hpp>
#include <Factory.hpp>
#include <InMemoryDatabase.hpp>
#include <c2xerror.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace c2x;
using namespace c2x::cam;

TEST(Test_InMemoryDatabase, Test_Factory_Function) {

  ASSERT_EQ(0, Factory::database().entryCount());
}

TEST(Test_InMemoryDatabase, Test_Create_Entry) {

  InMemoryDatabase db;

  auto entry = db.create(1);

  ASSERT_EQ(1, entry->header.stationID);
}

TEST(Test_InMemoryDatabase, Test_Create_Duplicate_Entry) {

  InMemoryDatabase db;
  int errorCode = 0;

  db.create(1);

  try {
    db.create(1);
  } catch (const DBException &e) {
    errorCode = e.getErrorCode();
  }

  ASSERT_EQ(ERR_CAM_ALREADY_EXISTS, errorCode);
}

TEST(Test_InMemoryDatabase, Test_Create_And_Get) {

  InMemoryDatabase db;

  db.create(1);
  auto view = db.get(1);

  ASSERT_EQ(1, view->header.stationID);
}

TEST(Test_InMemoryDatabase, Test_Create_And_Delete) {

  InMemoryDatabase db;

  db.create(1);
  db.remove(1);

  ASSERT_EQ(0, db.entryCount());
}

TEST(Test_InMemoryDatabase, Test_Modify_Id) {

  int testValue = 17;
  InMemoryDatabase db;

  db.create(1);
  {
    auto view = db.get(1);
    view->header.messageID = testValue;
    view->header.stationID = 2;
  }
  auto view = db.get(2);

  ASSERT_EQ(testValue, view->header.messageID);
}

TEST(Test_InMemoryDatabase, Test_Exists_Method) {
  InMemoryDatabase db;

  db.create(1);

  ASSERT_TRUE(db.exists(1));
}

TEST(Test_InMemoryDatabase, Test_Override_Entry_By_Modifying_StationID) {

  InMemoryDatabase db;

  db.create(1);
  db.create(2);
  {
    auto view = db.get(1);
    view->header.stationID = 2;
  }

  ASSERT_FALSE(db.exists(1));
  ASSERT_TRUE(db.exists(2));
  ASSERT_EQ(1, db.entryCount());
}