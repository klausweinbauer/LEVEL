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

#include <Database.hpp>
#include <Mocks.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

using ::testing::_;
using ::testing::Eq;
using ::testing::NiceMock;
using ::testing::Ref;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::Throw;

template <typename T>
std::unique_ptr<NiceMock<MIndexer<T>>> Database_getIndexer() {
  auto indexer = std::make_unique<NiceMock<MIndexer<T>>>();
  ON_CALL(*indexer, supportsQuery(_)).WillByDefault(Return(true));
  ON_CALL(*indexer, isValid(_, _)).WillByDefault(Return(true));
  return indexer;
}

std::shared_ptr<NiceMock<MQuery>> Database_getQuery() {
  return std::make_shared<NiceMock<MQuery>>();
}

template <typename T> DBView<T> insertAndGet(Database<T> &db, T value) {
  auto query = Database_getQuery();
  auto indexer1 = Database_getIndexer<int>();
  auto indexer2 = Database_getIndexer<int>();
  unsigned int index = 1234;
  EXPECT_CALL(*indexer1, addData(_, _)).WillRepeatedly(SaveArg<1>(&index));
  db.addIndexer(std::move(indexer1));
  db.insert(value);
  EXPECT_CALL(*indexer2, getIndexList(_))
      .WillOnce(Return(std::vector<unsigned int>({index})))
      .WillRepeatedly(Return(std::vector<unsigned int>()));
  db.addIndexer(std::move(indexer2));
  return std::move(db.get(query)[0]);
}

TEST(Database, DefaultCount) {
  Database<int> db;
  ASSERT_EQ(0, db.count());
}

TEST(Database, InsertByValue) {
  Database<int> db;
  int value = rand();
  auto view = db.insert(value);
  ASSERT_EQ(value, *view);
  ASSERT_EQ(1, db.count());
}

TEST(Database, InsertByReference) {
  Database<Dummy> db;
  int value = rand();
  auto view = db.insert(std::make_unique<Dummy>(value));
  ASSERT_EQ(value, view->_value);
  ASSERT_EQ(1, db.count());
}

TEST(Database, CallAddValueOnIndexerDuringInsert) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  int value = rand();
  EXPECT_CALL(*indexer, addData(value, 0)).Times(1);
  db.addIndexer(std::move(indexer));
  db.insert(value);
}

TEST(Database, IgnoreExceptionInAddValueByIndexer) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, addData(_, 0)).WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
  EXPECT_NO_THROW(db.insert(rand()));
}

TEST(Database, DoubleFreeOfSameView) {
  Database<int> db;
  auto view = db.insert(rand());
  ASSERT_TRUE(db.remove(view));
  ASSERT_FALSE(db.remove(view));
}

TEST(Database, CheckReuseOfIndices) {
  Database<int> db;
  int index1, index2;
  {
    auto view = db.insert(rand());
    index1 = view.index();
    db.remove(view);
  }
  {
    auto view = db.insert(rand());
    index2 = view.index();
    db.remove(view);
  }
  ASSERT_EQ(0, index1);
  ASSERT_EQ(0, index2);
}

TEST(Database, RemoveByViewCall) {
  Database<int> db;
  auto view = db.insert(rand());
  view.remove();
  ASSERT_NO_THROW(db.remove(view));
  ASSERT_THROW(view.index(), DBException);
}

TEST(Database, RemoveByDatabaseCall) {
  Database<int> db;
  auto view = db.insert(rand());
  db.remove(view);
  ASSERT_NO_THROW(view.remove());
  ASSERT_THROW(view.index(), DBException);
}

TEST(Database, CallRemoveValueOnIndexerDuringDeletion) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  int value = rand();
  EXPECT_CALL(*indexer, removeData(value, 0)).Times(1);
  db.addIndexer(std::move(indexer));
  db.remove(db.insert(value));
}

TEST(Database, CallRemoveValueOnIndexerDuringDeletionWhenInvokedOnView) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  int value = rand();
  EXPECT_CALL(*indexer, removeData(value, 0)).Times(1);
  db.addIndexer(std::move(indexer));
  auto view = db.insert(value);
  EXPECT_NO_THROW(view.remove());
}

TEST(Database, IgnoreExceptionInRemoveValueByIndexer) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, removeData(_, 0)).WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
  EXPECT_NO_THROW(db.remove(db.insert(rand())));
}

TEST(Database, GetNothing) {
  Database<int> db;
  auto query = Database_getQuery();
  auto result = db.get(query);
  ASSERT_EQ(0, result.size());
}

TEST(Database, CallToGetOnIndexerDuringGet) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  db.addIndexer(std::move(indexer));
  db.get(query);
}

TEST(Database, CallOnlyMatchingIndexerDuringGet) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, supportsQuery(Eq(query))).WillOnce(Return(false));
  EXPECT_CALL(*indexer, getIndexList(Eq(query))).Times(0);
  db.addIndexer(std::move(indexer));
  db.get(query);
}

TEST(Database, IgnoreExceptionInGetByIndexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
  EXPECT_NO_THROW(db.get(query));
}

TEST(Database, GetSelectedViewByIndexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  db.addIndexer(std::move(indexer));
  int value = rand();
  db.insert(value);
  auto result = db.get(query);
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(value, *result[0]);
}

TEST(Database, GetOnlyOneViewForMultipleSameIndicesByOneIndexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0, 0})));
  db.addIndexer(std::move(indexer));
  int value = rand();
  db.insert(value);
  auto result = db.get(query);
  ASSERT_EQ(1, result.size());
}

TEST(Database, GetOnlyOneViewForMultipleSameIndicesByMultipleIndexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer1 = Database_getIndexer<int>();
  auto indexer2 = Database_getIndexer<int>();
  EXPECT_CALL(*indexer1, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  EXPECT_CALL(*indexer2, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  db.addIndexer(std::move(indexer1));
  db.addIndexer(std::move(indexer2));
  db.insert(rand());
  auto result = db.get(query);
  ASSERT_EQ(1, result.size());
}

TEST(Database, IgnoreInvalidIndicesReturnedByIndexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0, 1, 2})));
  db.addIndexer(std::move(indexer));
  auto result = db.get(query);
  ASSERT_EQ(0, result.size());
}

TEST(Database, IgnoreEmptyIndicesReturnedByIndexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  db.addIndexer(std::move(indexer));
  db.remove(db.insert(rand()));
  auto result = db.get(query);
  ASSERT_EQ(0, result.size());
}

TEST(Database, ReturnViewsInCorrectOrderIfSwitchedByIndexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({1, 0})));
  db.addIndexer(std::move(indexer));
  int value1 = rand(), value2 = rand();
  db.insert(value1);
  db.insert(value2);
  auto result = db.get(query);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(value1, *result[0]);
  ASSERT_EQ(value2, *result[1]);
}

TEST(Database, InsertAndGet) {
  Database<int> db;
  int data1 = rand();
  int data2 = rand();
  auto view1 = insertAndGet<int>(db, data1);
  auto view2 = insertAndGet<int>(db, data2);
  ASSERT_EQ(data1, *view1);
  ASSERT_EQ(data2, *view2);
}

TEST(Database, CallUpdateDataOnIndexerAfterViewScopeEnds) {
  Database<int> db;
  auto indexer1 = Database_getIndexer<int>();
  auto indexer2 = Database_getIndexer<int>();
  auto query = Database_getQuery();
  EXPECT_CALL(*indexer1, getIndexList(_))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  db.addIndexer(std::move(indexer1));
  db.insert(rand());
  EXPECT_CALL(*indexer2, updateData(_, 0)).Times(1);
  db.addIndexer(std::move(indexer2));
  db.get(query);
}

TEST(Database, CheckIsValidStateWithIndexerDuringGet) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  auto query = Database_getQuery();
  int value = rand();
  EXPECT_CALL(*indexer, getIndexList(_))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  EXPECT_CALL(*indexer, isValid(value, 0)).WillOnce(Return(true));
  db.addIndexer(std::move(indexer));
  db.insert(value);
  db.get(query);
}

TEST(Database, DoNotCheckIsValidStateWithNotSupportedIndexerDuringGet) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  auto query = Database_getQuery();
  int value = rand();
  EXPECT_CALL(*indexer, supportsQuery(_))
      .WillOnce(Return(true))
      .WillRepeatedly(Return(false));
  EXPECT_CALL(*indexer, getIndexList(_))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  EXPECT_CALL(*indexer, isValid(value, 0)).Times(0);
  db.addIndexer(std::move(indexer));
  db.insert(value);
  db.get(query);
}

TEST(Database, HandleExceptionFromIsValidDuringGet) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  auto query = Database_getQuery();
  int value = rand();
  EXPECT_CALL(*indexer, getIndexList(_))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  EXPECT_CALL(*indexer, isValid(value, 0)).WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
  db.insert(value);
  ASSERT_NO_THROW(db.get(query));
}

TEST(Database, HandleExceptionFromSupportsQueryDuringIsValidCheck) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  auto query = Database_getQuery();
  int value = rand();
  EXPECT_CALL(*indexer, getIndexList(_))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  EXPECT_CALL(*indexer, supportsQuery(_))
      .WillOnce(Return(true))
      .WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
  db.insert(value);
  ASSERT_NO_THROW(db.get(query));
}

TEST(Database, HandleExceptionFromSupportsQueryDuringGet) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  auto query = Database_getQuery();
  EXPECT_CALL(*indexer, supportsQuery(_)).WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
  ASSERT_NO_THROW(db.get(query));
}

TEST(Database, ExceptionOnAddNullptrIndexer) {
  Database<int> db;
  auto indexer = std::unique_ptr<NiceMock<MIndexer<int>>>(nullptr);
  ASSERT_THROW(db.addIndexer(std::move(indexer)), Exception);
}