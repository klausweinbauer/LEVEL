#include <DBElement.hpp>
#include <DBView.hpp>
#include <Mocks.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

using ::testing::ByRef;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnPointee;

// DBView(DBElement<T> *element, DBElementStatus *status)
// virtual ~DBView()
// DBView(DBView<T> &&view) : _element(nullptr), _status(nullptr)
// DBView<T> &operator=(DBView<T> &&view)
// T &operator->()
// T &operator*()

struct DBViewTestHelper {
  int _value;
};

TEST(DBView, Initialize_With_Element_Nullptr) {
  ASSERT_THROW(DBView<int> view(nullptr), Exception);
}

TEST(DBView, Lock_Element_During_Construction) {
  auto element = std::make_shared<MDBElement<int>>();
  EXPECT_CALL(*element, lock()).Times(1);
  DBView<int> view(element);
}

TEST(DBView, Unlock_Element_At_End_Of_Life) {
  auto element = std::make_shared<MDBElement<int>>();
  EXPECT_CALL(*element, unlock()).Times(1);
  { DBView<int> view(element); }
}

TEST(DBView, Do_Not_Unlock_Element_At_End_Of_Life_When_Deleted) {
  auto element = std::make_shared<MDBElement<int>>();
  EXPECT_CALL(*element, unlock()).Times(0);
  {
    DBView<int> view(element);
    view.remove();
  }
}

TEST(DBView, Call_Clear_On_DBElement_During_Delete) {
  auto element = std::make_shared<MDBElement<int>>();
  EXPECT_CALL(*element, clear()).Times(1);
  DBView<int> view(element);
  view.remove();
}

TEST(DBView, Move_Construction) {
  auto element = std::make_shared<DBElement<int>>(0);
  DBView<int> view1(element);
  DBView<int> view2(std::move(view1));
  ASSERT_FALSE(view1.isValid());
}

TEST(DBView, Move_Assignment) {
  auto element1 = std::make_shared<DBElement<int>>(0);
  auto element2 = std::make_shared<DBElement<int>>(0);
  DBView<int> view1(element1);
  DBView<int> view2(element2);
  view2 = std::move(view1);
}

TEST(DBView, Check_Valid_State) {
  auto element = std::make_shared<DBElement<int>>(0);
  DBView<int> view(element);
  ASSERT_TRUE(view.isValid());
}

TEST(DBView, Check_Valid_State_After_Delete) {
  auto element = std::make_shared<DBElement<int>>(0);
  DBView<int> view(element);
  view.remove();
  ASSERT_FALSE(view.isValid());
}

TEST(DBView, Deref_View) {
  int value = rand();
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  EXPECT_CALL(*element, data()).WillRepeatedly(ReturnPointee(&value));
  DBView<int> view(element);
  ASSERT_EQ(value, *view);
}

TEST(DBView, Deref_View_By_Arrow) {
  DBViewTestHelper helper = {._value = rand()};
  auto element = std::make_shared<NiceMock<MDBElement<DBViewTestHelper>>>();
  EXPECT_CALL(*element, data()).WillRepeatedly(ReturnPointee(&helper));
  DBView<DBViewTestHelper> view(element);
  ASSERT_EQ(helper._value, view->_value);
}

TEST(DBView, Modify_By_Reference) {
  int value = rand();
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  EXPECT_CALL(*element, data()).WillRepeatedly(ReturnPointee(&value));
  DBView<int> view(element);
  *view = 1;
  ASSERT_EQ(1, value);
}

TEST(DBView, Modify_By_Arrow_Reference) {
  DBViewTestHelper helper = {._value = rand()};
  auto element = std::make_shared<NiceMock<MDBElement<DBViewTestHelper>>>();
  EXPECT_CALL(*element, data()).WillRepeatedly(ReturnPointee(&helper));
  DBView<DBViewTestHelper> view(element);
  view->_value = 1;
  ASSERT_EQ(1, helper._value);
}

TEST(DBView, Throw_Exception_When_Deref_Invalid) {
  auto element = std::make_shared<DBElement<int>>(0);
  DBView<int> view(element);
  view.remove();
  ASSERT_THROW(*view, DBException);
}

TEST(DBView, Throw_Exception_When_Deref_Invalid_By_Arrow) {
  auto element = std::make_shared<DBElement<DBViewTestHelper>>(0);
  DBView<DBViewTestHelper> view(element);
  view.remove();
  ASSERT_THROW(view->_value, DBException);
}