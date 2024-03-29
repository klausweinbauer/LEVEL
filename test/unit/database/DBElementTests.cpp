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

TEST(DBElement, CorrectIndexInitialization) {
  unsigned int index = (unsigned int)rand();
  DBElement<int> element(index);
  ASSERT_EQ(index, element.index());
}

TEST(DBElement, SetHoldingThreadIdAfterLock) {
  DBElement<int> element;
  element.lock();
  ASSERT_EQ(std::this_thread::get_id(), element.holdingThread());
  element.unlock();
}

TEST(DBElement, ResetHoldingThreadIdAfterUnlock) {
  DBElement<int> element;
  element.lock();
  element.unlock();
  ASSERT_EQ(std::thread::id(), element.holdingThread());
}

TEST(DBElement, SetAndGetData) {
  DBElement<DBElementTestHelper> element;
  int value = rand();
  element.setData(std::move(std::make_unique<DBElementTestHelper>(value)));
  element.data();
  ASSERT_EQ(value, element.data()._value);
}

TEST(DBElement, ThrowExceptionOnDataWhenNotSet) {
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

TEST(DBElement, CallToDatabaseOnClear) {
  auto db = std::make_shared<NiceMock<MDatabase<int>>>();
  DBElement<int> element;
  DBElement_MRemoveCallback<int> callback;
  element.removeCallback = [&callback](DBElement<int> *element) {
    callback.removeCallback(element);
  };
  EXPECT_CALL(callback, removeCallback(&element)).Times(1);
  element.clear();
}

TEST(DBElement, IgnoreCallToDatabaseOnClearIfNotInitialized) {
  unsigned int index = (unsigned int)rand();
  DBElement<int> element(index);
  ASSERT_NO_THROW(element.clear());
}

TEST(DBElement, HasDataAssigned) {
  DBElement<int> element;
  element.setData(std::make_unique<int>(rand()));
  ASSERT_TRUE(element.hasData());
}

TEST(DBElement, HasNoDataAssigned) {
  DBElement<int> element;
  ASSERT_FALSE(element.hasData());
}

template <typename T> class DBElement_MUpdateCallback {
public:
  DBElement_MUpdateCallback() {}

  MOCK_METHOD(void, updateCallback, (const DBElement<T> *const));
};

TEST(DBElement, CallDataModifiedWhenUnlocked) {
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

TEST(DBElement, DoNotCallDataModifiedWhenUnlockedAndDataIsNull) {
  DBElement<int> element;
  DBElement_MUpdateCallback<int> callback;
  element.updateCallback = [&callback](const DBElement<int> *const e) {
    callback.updateCallback(e);
  };
  element.lock();
  EXPECT_CALL(callback, updateCallback(&element)).Times(0);
  element.unlock();
}

TEST(DBElement, DoNotCallDataModifiedWhenNotSet) {
  DBElement<int> element;
  element.setData(std::make_unique<int>(rand()));
  DBElement_MUpdateCallback<int> callback;
  element.lock();
  EXPECT_CALL(callback, updateCallback(&element)).Times(0);
  ASSERT_NO_THROW(element.unlock());
}