#include <DBElement.hpp>
#include <Mocks.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::SaveArg;

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
  DBElement<int> element(index, db.get());
  EXPECT_CALL(*db, remove(index)).Times(1);
  element.clear();
}

TEST(DBElement, Ignore_Call_To_Database_On_Clear_If_Not_Initialized) {
  unsigned int index = (unsigned int)rand();
  DBElement<int> element(index, nullptr);
  ASSERT_NO_THROW(element.clear());
}

TEST(DBElement, Has_Data_Assigned) {
  DBElement<int> element;
  element.setData(std::make_unique<int>(rand()));
  ASSERT_TRUE(element.hasData());
}

TEST(DBElement, Has_No_Data_Assigned) {
  DBElement<int> element;
  ASSERT_FALSE(element.hasData());
}

template <typename T> class DBElement_MDataChanged {
public:
  DBElement_MDataChanged() {}

  MOCK_METHOD(void, dataChanged, (const DBElement<T> *const));
};

TEST(DBElement, Call_Data_Modified_When_Unlocked) {
  DBElement<int> element;
  element.setData(std::make_unique<int>(rand()));
  DBElement_MDataChanged<int> callback;
  element.dataChanged = [&callback](const DBElement<int> *const e) {
    callback.dataChanged(e);
  };
  EXPECT_CALL(callback, dataChanged(&element)).Times(1);
  element.unlock();
}

TEST(DBElement, Do_Not_Call_Data_Modified_When_Unlocked_And_Data_Is_Null) {
  DBElement<int> element;
  DBElement_MDataChanged<int> callback;
  element.dataChanged = [&callback](const DBElement<int> *const e) {
    callback.dataChanged(e);
  };
  EXPECT_CALL(callback, dataChanged(&element)).Times(0);
  element.unlock();
}

TEST(DBElement, Do_Not_Call_Data_Modified_When_Not_Set) {
  DBElement<int> element;
  element.setData(std::make_unique<int>(rand()));
  DBElement_MDataChanged<int> callback;
  EXPECT_CALL(callback, dataChanged(&element)).Times(0);
  ASSERT_NO_THROW(element.unlock());
}