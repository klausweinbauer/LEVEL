#include <DBElement.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

struct DBElementTestHelper {
  int _value;

  DBElementTestHelper() {}
  DBElementTestHelper(int value) : _value(value) {}
};

TEST(DBElement, Correct_Index_Initialization) {
  unsigned int index = (unsigned int)rand();
  DBElement<int> element(index);
  ASSERT_EQ(index, element.index());
}

TEST(DBElement, Set_Holding_Thread_Id_After_Lock) {
  DBElement<int> element(0);
  element.lock();
  ASSERT_EQ(std::this_thread::get_id(), element.holdingThread());
}

TEST(DBElement, Reset_Holding_Thread_Id_After_Unlock) {
  DBElement<int> element(0);
  element.lock();
  element.unlock();
  ASSERT_EQ(std::thread::id(), element.holdingThread());
}

TEST(DBElement, Set_And_Get_Data) {
  DBElement<DBElementTestHelper> element(0);
  int value = rand();
  element.setData(std::move(std::make_unique<DBElementTestHelper>(value)));
  element.data();
  ASSERT_EQ(value, element.data()._value);
}

TEST(DBElement, Throw_Exception_On_Data_When_Not_Set) {
  DBElement<DBElementTestHelper> element(0);
  ASSERT_THROW(element.data()._value = rand(), DBException);
}

TEST(DBElement, Clear) {
  DBElement<DBElementTestHelper> element(0);
  int value = rand();
  element.setData(std::move(std::make_unique<DBElementTestHelper>(value)));
  element.clear();
  ASSERT_THROW(element.data(), DBException);
}