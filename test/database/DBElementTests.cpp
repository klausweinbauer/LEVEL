#include <DBElement.hpp>
#include <Mocks.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

using ::testing::_;
using ::testing::NiceMock;

struct DBElementTestHelper {
  int _value;

  DBElementTestHelper() {}
  DBElementTestHelper(int value) : _value(value) {}
};

TEST(DBElement, Correct_Index_Initialization) {
  unsigned int index = (unsigned int)rand();
  DBElement<int> element(index, nullptr);
  ASSERT_EQ(index, element.index());
}

TEST(DBElement, Set_Holding_Thread_Id_After_Lock) {
  DBElement<int> element;
  element.lock();
  ASSERT_EQ(std::this_thread::get_id(), element.holdingThread());
}

TEST(DBElement, Reset_Holding_Thread_Id_After_Unlock) {
  DBElement<int> element;
  element.lock();
  element.unlock();
  ASSERT_EQ(std::thread::id(), element.holdingThread());
}

TEST(DBElement, Set_And_Get_Data) {
  DBElement<DBElementTestHelper> element;
  int value = rand();
  element.setData(std::move(std::make_unique<DBElementTestHelper>(value)));
  element.data();
  ASSERT_EQ(value, element.data()._value);
}

TEST(DBElement, Throw_Exception_On_Data_When_Not_Set) {
  DBElement<DBElementTestHelper> element;
  ASSERT_THROW(element.data()._value = rand(), DBException);
}

TEST(DBElement, Clear) {
  DBElement<DBElementTestHelper> element;
  int value = rand();
  element.setData(std::move(std::make_unique<DBElementTestHelper>(value)));
  element.clear();
  ASSERT_THROW(element.data(), DBException);
}

TEST(DBElement, Call_To_Database_On_Clear) {
  auto db = std::make_shared<NiceMock<MDatabase<int>>>();
  unsigned int index = (unsigned int)rand();
  DBElement<int> element(index, db);
  EXPECT_CALL(*db, remove(index)).Times(1);
  element.clear();
}

TEST(DBElement, Ignore_Call_To_Database_On_Clear_If_Not_Initialized) {
  unsigned int index = (unsigned int)rand();
  DBElement<int> element(index, nullptr);
  ASSERT_NO_THROW(element.clear());
}