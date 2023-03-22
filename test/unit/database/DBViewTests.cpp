// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

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

struct DBViewTestHelper {
  int _value;
  DBViewTestHelper() : _value(0) {}
  DBViewTestHelper(int value) : _value(value) {}
};

TEST(DBView, InitializeWithElementNullptr) {
  ASSERT_THROW(DBView<int> view(nullptr), Exception);
}

TEST(DBView, LockElementDuringConstruction) {
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  EXPECT_CALL(*element, lock()).Times(1);
  DBView<int> view(element.get());
}

TEST(DBView, UnlockElementAtEndOfLife) {
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  EXPECT_CALL(*element, unlock()).Times(1);
  { DBView<int> view(element.get()); }
}

TEST(DBView, DoNotUnlockElementAtEndOfLifeWhenDeleted) {
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  {
    DBView<int> view(element.get());
    view.remove();
    EXPECT_CALL(*element, unlock()).Times(0);
  }
}

TEST(DBView, CallClearOnDBElementDuringDelete) {
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  EXPECT_CALL(*element, clear()).Times(1);
  DBView<int> view(element.get());
  view.remove();
}

TEST(DBView, MoveConstruction) {
  auto element = std::make_shared<DBElement<int>>();
  DBView<int> view1(element.get());
  DBView<int> view2(std::move(view1));
  ASSERT_FALSE(view1.isValid());
}

TEST(DBView, MoveAssignment) {
  auto element1 = std::make_shared<DBElement<int>>();
  auto element2 = std::make_shared<DBElement<int>>();
  DBView<int> view1(element1.get());
  DBView<int> view2(element2.get());
  view2 = std::move(view1);
}

TEST(DBView, CheckValidState) {
  auto element = std::make_shared<DBElement<int>>();
  DBView<int> view(element.get());
  ASSERT_TRUE(view.isValid());
}

TEST(DBView, CheckValidStateAfterDelete) {
  auto element = std::make_shared<DBElement<int>>();
  DBView<int> view(element.get());
  view.remove();
  ASSERT_FALSE(view.isValid());
}

TEST(DBView, DerefView) {
  int value = rand();
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  EXPECT_CALL(*element, data()).WillRepeatedly(ReturnPointee(&value));
  DBView<int> view(element.get());
  ASSERT_EQ(value, *view);
}

TEST(DBView, DerefViewByArrow) {
  DBViewTestHelper helper(rand());
  auto element = std::make_shared<NiceMock<MDBElement<DBViewTestHelper>>>();
  EXPECT_CALL(*element, data()).WillRepeatedly(ReturnPointee(&helper));
  DBView<DBViewTestHelper> view(element.get());
  ASSERT_EQ(helper._value, view->_value);
}

TEST(DBView, ModifyByReference) {
  int value = rand();
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  EXPECT_CALL(*element, data()).WillRepeatedly(ReturnPointee(&value));
  DBView<int> view(element.get());
  *view = 1;
  ASSERT_EQ(1, value);
}

TEST(DBView, ModifyByArrowReference) {
  DBViewTestHelper helper(rand());
  auto element = std::make_shared<NiceMock<MDBElement<DBViewTestHelper>>>();
  EXPECT_CALL(*element, data()).WillRepeatedly(ReturnPointee(&helper));
  DBView<DBViewTestHelper> view(element.get());
  view->_value = 1;
  ASSERT_EQ(1, helper._value);
}

TEST(DBView, ThrowExceptionWhenDerefInvalid) {
  auto element = std::make_shared<DBElement<int>>();
  DBView<int> view(element.get());
  view.remove();
  ASSERT_THROW(*view, DBException);
}

TEST(DBView, Throw_Exception_When_Deref_Invalid_By_Arrow) {
  auto element = std::make_shared<DBElement<DBViewTestHelper>>();
  DBView<DBViewTestHelper> view(element.get());
  view.remove();
  ASSERT_THROW(view->_value, DBException);
}

TEST(DBView, GetIndex) {
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  DBView<int> view(element.get());
  unsigned int index = (unsigned int)rand();
  EXPECT_CALL(*element, index()).WillOnce(Return(index));
  EXPECT_EQ(index, view.index());
}

TEST(DBView, GetIndexThrowsExceptionWhenViewIsInvalid) {
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  DBView<int> view(element.get());
  view.remove();
  EXPECT_THROW(view.index(), DBException);
}

TEST(DBView, DoubleRemoveCall) {
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  DBView<int> view(element.get());
  view.remove();
  ASSERT_NO_THROW(view.remove());
}

TEST(DBView, UnlockElementInRemoveCall) {
  auto element = std::make_shared<NiceMock<MDBElement<int>>>();
  DBView<int> view(element.get());
  EXPECT_CALL(*element, unlock()).Times(1);
  view.remove();
}