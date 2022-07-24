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
using ::testing::Throw;

template <typename T>
std::shared_ptr<NiceMock<MIndexer<T>>> Database_getIndexer() {
  auto indexer = std::make_shared<NiceMock<MIndexer<T>>>();
  ON_CALL(*indexer, supportsQuery(_)).WillByDefault(Return(true));
  return indexer;
}

std::shared_ptr<NiceMock<MQuery>> Database_getQuery() {
  return std::make_shared<NiceMock<MQuery>>();
}

TEST(Database, Default_Count) {
  Database<int> db;
  ASSERT_EQ(0, db.count());
}

TEST(Database, Insert_By_Value) {
  Database<int> db;
  int value = rand();
  auto view = db.insert(value);
  ASSERT_EQ(value, *view);
  ASSERT_EQ(1, db.count());
}

TEST(Database, Insert_By_Reference) {
  Database<Dummy> db;
  int value = rand();
  auto view = db.insert(std::make_unique<Dummy>(value));
  ASSERT_EQ(value, view->_value);
  ASSERT_EQ(1, db.count());
}

TEST(Database, Call_Add_Value_On_Indexer_During_Insert) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  int value = rand();
  EXPECT_CALL(*indexer, addData(value, 0)).Times(1);
  db.insert(value);
}

TEST(Database, Ignore_Exception_In_Add_Value_By_Indexer) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  EXPECT_CALL(*indexer, addData(_, 0)).WillOnce(Throw(std::exception()));
  EXPECT_NO_THROW(db.insert(rand()));
}

TEST(Database, Exception_When_Removing_Not_Existing_Entry) {
  Database<int> db;
  ASSERT_THROW(db.remove(1), DBException);
}

TEST(Database, Exception_When_Removing_Deleted_Entry) {
  Database<int> db;
  db.insert(rand());
  db.remove(0);
  ASSERT_THROW(db.remove(0), DBException);
}

TEST(Database, Remove_Entry_By_Index) {
  Database<int> db;
  db.insert(rand());
  db.remove(0);
  ASSERT_EQ(0, db.count());
}

TEST(Database, Remove_Entry_By_Index_While_Holding_View) {
  Database<int> db;
  auto view = db.insert(rand());
  ASSERT_NO_THROW(db.remove(view.index()));
}

TEST(Database, Reuse_Of_Indices) {
  Database<int> db;
  db.insert(rand());
  db.remove(0);
  db.insert(rand());
  ASSERT_NO_THROW(db.remove(0));
}

TEST(Database, Remove_By_Direct_View_Call) {
  Database<int> db;
  auto view = db.insert(rand());
  view.remove();
  ASSERT_THROW(db.remove(0), DBException);
  ASSERT_THROW(view.index(), Exception);
}

TEST(Database, Remove_By_View) {
  Database<int> db;
  auto view = db.insert(rand());
  db.remove(view);
  ASSERT_THROW(db.remove(0), DBException);
  ASSERT_THROW(view.index(), Exception);
}

TEST(Database, Call_Remove_Value_On_Indexer_During_Deletion) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  int value = rand();
  EXPECT_CALL(*indexer, removeData(value, 0)).Times(1);
  db.remove(db.insert(value));
}

TEST(Database,
     Call_Remove_Value_On_Indexer_During_Deletion_When_Invoked_On_View) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  int value = rand();
  EXPECT_CALL(*indexer, removeData(value, 0)).Times(1);
  auto view = db.insert(value);
  EXPECT_NO_THROW(view.remove());
}

TEST(Database, Ignore_Exception_In_Remove_Value_By_Indexer) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  EXPECT_CALL(*indexer, removeData(_, 0)).WillOnce(Throw(std::exception()));
  EXPECT_NO_THROW(db.remove(db.insert(rand())));
}

TEST(Database, Get_Nothing) {
  Database<int> db;
  auto query = Database_getQuery();
  auto result = db.get(query);
  ASSERT_EQ(0, result.size());
}

TEST(Database, Call_To_Get_On_Indexer_During_Get) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  db.get(query);
}

TEST(Database, Call_Only_Matching_Indexer_During_Get) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  EXPECT_CALL(*indexer, supportsQuery(Eq(query))).WillOnce(Return(false));
  EXPECT_CALL(*indexer, getIndexList(Eq(query))).Times(0);
  db.get(query);
}

TEST(Database, Ignore_Exception_In_Get_By_Indexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Throw(std::exception()));
  EXPECT_NO_THROW(db.get(query));
}

TEST(Database, Get_Selected_View_By_Indexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  int value = rand();
  db.insert(value);
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  auto result = db.get(query);
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(value, *result[0]);
}

TEST(Database, Get_Only_One_View_For_Multiple_Same_Indices_By_One_Indexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  int value = rand();
  db.insert(value);
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0, 0})));
  auto result = db.get(query);
  ASSERT_EQ(1, result.size());
}

TEST(Database,
     Get_Only_One_View_For_Multiple_Same_Indices_By_Multiple_Indexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer1 = Database_getIndexer<int>();
  auto indexer2 = Database_getIndexer<int>();
  db.addIndexer(indexer1);
  db.addIndexer(indexer2);
  db.insert(rand());
  EXPECT_CALL(*indexer1, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  EXPECT_CALL(*indexer2, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  auto result = db.get(query);
  ASSERT_EQ(1, result.size());
}

TEST(Database, Ignore_Invalid_Indices_Returned_By_Indexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0, 1, 2})));
  auto result = db.get(query);
  ASSERT_EQ(0, result.size());
}

TEST(Database, Ignore_Empty_Indices_Returned_By_Indexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  db.remove(db.insert(rand()));
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  auto result = db.get(query);
  ASSERT_EQ(0, result.size());
}

TEST(Database, Return_Views_In_Correct_Order_If_Switched_By_Indexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  db.addIndexer(indexer);
  int value1 = rand(), value2 = rand();
  db.insert(value1);
  db.insert(value2);
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({1, 0})));
  auto result = db.get(query);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(value1, *result[0]);
  ASSERT_EQ(value2, *result[1]);
}