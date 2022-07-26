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
  DBElement<int> element(index);
  ASSERT_EQ(index, element.index());
}

TEST(DBElement, Set_Holding_Thread_Id_After_Lock) {
  DBElement<int> element;
  element.lock();
  ASSERT_EQ(std::this_thread::get_id(), element.holdingThread());
  element.unlock();
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

template <typename T> class DBElement_MRemoveCallback {
public:
  DBElement_MRemoveCallback() {}

  MOCK_METHOD(void, removeCallback, (DBElement<T> *));
};

TEST(DBElement, Call_To_Database_On_Clear) {
  auto db = std::make_shared<NiceMock<MDatabase<int>>>();
  DBElement<int> element;
  DBElement_MRemoveCallback<int> callback;
  element.removeCallback = [&callback](DBElement<int> *element) {
    callback.removeCallback(element);
  };
  EXPECT_CALL(callback, removeCallback(&element)).Times(1);
  element.clear();
}

TEST(DBElement, Ignore_Call_To_Database_On_Clear_If_Not_Initialized) {
  unsigned int index = (unsigned int)rand();
  DBElement<int> element(index);
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

template <typename T> class DBElement_MUpdateCallback {
public:
  DBElement_MUpdateCallback() {}

  MOCK_METHOD(void, updateCallback, (const DBElement<T> *const));
};

TEST(DBElement, Call_Data_Modified_When_Unlocked) {
  DBElement<int> element;
  element.setData(std::make_unique<int>(rand()));
  DBElement_MUpdateCallback<int> callback;
  element.updateCallback = [&callback](const DBElement<int> *const e) {
    callback.updateCallback(e);
  };
  element.lock();
  EXPECT_CALL(callback, updateCallback(&element)).Times(1);
  element.unlock();
}

TEST(DBElement, Do_Not_Call_Data_Modified_When_Unlocked_And_Data_Is_Null) {
  DBElement<int> element;
  DBElement_MUpdateCallback<int> callback;
  element.updateCallback = [&callback](const DBElement<int> *const e) {
    callback.updateCallback(e);
  };
  element.lock();
  EXPECT_CALL(callback, updateCallback(&element)).Times(0);
  element.unlock();
}

TEST(DBElement, Do_Not_Call_Data_Modified_When_Not_Set) {
  DBElement<int> element;
  element.setData(std::make_unique<int>(rand()));
  DBElement_MUpdateCallback<int> callback;
  element.lock();
  EXPECT_CALL(callback, updateCallback(&element)).Times(0);
  ASSERT_NO_THROW(element.unlock());
}