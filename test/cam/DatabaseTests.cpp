#include <CAMFactory.hpp>
#include <DBException.hpp>
#include <InMemoryDatabase.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <level.h>
#include <thread>

using namespace level;
using namespace level::cam;

template <typename T> T getRandParam() { return (T)rand(); }

long unsigned int getRandId() { return getRandParam<long unsigned int>(); }

std::shared_ptr<IDatabase> getDBInstance() {
  return std::shared_ptr<IDatabase>(new InMemoryDatabase());
}

TEST(CAM_DatabaseTests, Test_Factory_Function) {

  ASSERT_EQ(0, Factory::db().entryCount());
}

TEST(CAM_DatabaseTests, Test_Create_Entry) {

  auto db = getDBInstance();
  auto id = getRandId();

  auto entry = db->create(id);

  ASSERT_EQ(id, entry->cam()->header.stationID);
}

TEST(CAM_DatabaseTests, Test_Create_Duplicate_Entry) {

  auto db = getDBInstance();
  auto id = getRandId();
  int errorCode = 0;

  db->create(id);

  try {
    db->create(id);
  } catch (const DBException &e) {
    errorCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_CAM_ALREADY_EXISTS, errorCode);
}

TEST(CAM_DatabaseTests, Test_Create_And_Get) {

  auto db = getDBInstance();
  auto id = getRandId();

  db->create(id);
  auto view = db->get(id);

  ASSERT_EQ(id, view->cam()->header.stationID);
}

TEST(CAM_DatabaseTests, Test_Create_And_Delete) {

  auto db = getDBInstance();
  auto id = getRandId();

  db->create(id);
  db->remove(id);

  ASSERT_EQ(0, db->entryCount());
}

TEST(CAM_DatabaseTests, Test_Modify_Id) {

  auto db = getDBInstance();
  auto id1 = getRandId();
  auto id2 = getRandId();
  CAMData *cam1, *cam2;

  db->create(id1);
  {
    auto view = db->get(id1);
    cam1 = *view;
    view->cam()->header.stationID = id2;
  }
  auto view = db->get(id2);
  cam2 = *view;

  ASSERT_EQ(cam1, cam2);
}

TEST(CAM_DatabaseTests, Test_Exists_Method) {

  auto db = getDBInstance();
  auto id = getRandId();

  db->create(id);

  ASSERT_TRUE(db->exists(id));
}

TEST(CAM_DatabaseTests, Test_Override_Entry_By_Modifying_StationID) {

  auto db = getDBInstance();
  auto id1 = getRandId();
  auto id2 = id1 + 1;

  db->create(id1);
  db->create(id2);
  {
    auto view = db->get(id1);
    view->cam()->header.stationID = id2;
  }

  ASSERT_FALSE(db->exists(id1));
  ASSERT_TRUE(db->exists(id2));
  ASSERT_EQ(1, db->entryCount());
}

TEST(CAM_DatabaseTests, Test_Exception_When_Opening_Same_View_Multiple_Timest) {

  auto db = getDBInstance();
  auto id = getRandId();
  int errorCode = 0;

  db->create(id);
  auto view1 = db->get(id);

  try {
    auto view2 = db->get(id);
  } catch (const DBException &e) {
    errorCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_INVALID_OPERATION, errorCode);
}

TEST(CAM_DatabaseTests,
     Test_Exception_When_Opening_Different_Views_Concurrently) {

  auto db = getDBInstance();
  auto id1 = getRandId();
  auto id2 = id1 + 1;
  int errorCode = 0;

  db->create(id1);
  db->create(id2);

  auto view1 = db->get(id1);
  try {
    auto view2 = db->get(id2);
  } catch (const DBException &e) {
    errorCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_INVALID_OPERATION, errorCode);
}

TEST(CAM_DatabaseTests, Test_Get_Multiple_Views) {

  auto db = getDBInstance();
  auto id = getRandId();

  db->create(id);
  for (size_t i = 0; i < 5; i++) {
    {
      auto view = db->get(id);
    }
  }

  ASSERT_TRUE(true); // When we get here, everything went right -> no core dump
}

void incMessageId(std::shared_ptr<IDatabase> db, long unsigned int id,
                  int incCount) {

  for (int i = 0; i < incCount; i++) {
    {
      auto view = db->get(id);
      view->cam()->header.messageID++;
    }
  }
}

TEST(CAM_DatabaseTests, Test_For_Race_Condition_In_DB_Access) {

  int nrThreads = 10;
  int incPerThread = 10000;
  auto db = getDBInstance();
  auto id = getRandId();

  std::thread threads[nrThreads];
  db->create(id);

  for (int i = 0; i < nrThreads; i++) {
    threads[i] = std::thread(incMessageId, db, id, incPerThread);
  }

  for (int i = 0; i < nrThreads; i++) {
    threads[i].join();
  }

  int finalCount = db->get(id)->cam()->header.messageID;

  ASSERT_EQ(nrThreads * incPerThread, finalCount);
}