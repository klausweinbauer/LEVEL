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
  } catch (DBException &e) {
    errorCode = e.getErrorCode();
  }

  ASSERT_EQ(ERR_CAM_ALREADY_EXISTS, errorCode);
}