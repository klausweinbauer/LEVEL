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
  int value = rand();
  EXPECT_CALL(*indexer, addData(value, 0)).Times(1);
  db.addIndexer(std::move(indexer));
  db.insert(value);
}

TEST(Database, Ignore_Exception_In_Add_Value_By_Indexer) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, addData(_, 0)).WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
  EXPECT_NO_THROW(db.insert(rand()));
}

TEST(Database, Double_Free_Of_Same_View) {
  Database<int> db;
  auto view = db.insert(rand());
  ASSERT_TRUE(db.remove(view));
  ASSERT_FALSE(db.remove(view));
}

TEST(Database, Check_Reuse_Of_Indices) {
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

TEST(Database, Remove_By_View_Call) {
  Database<int> db;
  auto view = db.insert(rand());
  view.remove();
  ASSERT_NO_THROW(db.remove(view));
  ASSERT_THROW(view.index(), DBException);
}

TEST(Database, Remove_By_Database_Call) {
  Database<int> db;
  auto view = db.insert(rand());
  db.remove(view);
  ASSERT_NO_THROW(view.remove());
  ASSERT_THROW(view.index(), DBException);
}

TEST(Database, Call_Remove_Value_On_Indexer_During_Deletion) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  int value = rand();
  EXPECT_CALL(*indexer, removeData(value, 0)).Times(1);
  db.addIndexer(std::move(indexer));
  db.remove(db.insert(value));
}

TEST(Database,
     Call_Remove_Value_On_Indexer_During_Deletion_When_Invoked_On_View) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  int value = rand();
  EXPECT_CALL(*indexer, removeData(value, 0)).Times(1);
  db.addIndexer(std::move(indexer));
  auto view = db.insert(value);
  EXPECT_NO_THROW(view.remove());
}

TEST(Database, Ignore_Exception_In_Remove_Value_By_Indexer) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, removeData(_, 0)).WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
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
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0})));
  db.addIndexer(std::move(indexer));
  db.get(query);
}

TEST(Database, Call_Only_Matching_Indexer_During_Get) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, supportsQuery(Eq(query))).WillOnce(Return(false));
  EXPECT_CALL(*indexer, getIndexList(Eq(query))).Times(0);
  db.addIndexer(std::move(indexer));
  db.get(query);
}

TEST(Database, Ignore_Exception_In_Get_By_Indexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
  EXPECT_NO_THROW(db.get(query));
}

TEST(Database, Get_Selected_View_By_Indexer) {
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

TEST(Database, Get_Only_One_View_For_Multiple_Same_Indices_By_One_Indexer) {
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

TEST(Database,
     Get_Only_One_View_For_Multiple_Same_Indices_By_Multiple_Indexer) {
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

TEST(Database, Ignore_Invalid_Indices_Returned_By_Indexer) {
  Database<int> db;
  auto query = Database_getQuery();
  auto indexer = Database_getIndexer<int>();
  EXPECT_CALL(*indexer, getIndexList(Eq(query)))
      .WillOnce(Return(std::vector<unsigned int>({0, 1, 2})));
  db.addIndexer(std::move(indexer));
  auto result = db.get(query);
  ASSERT_EQ(0, result.size());
}

TEST(Database, Ignore_Empty_Indices_Returned_By_Indexer) {
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

TEST(Database, Return_Views_In_Correct_Order_If_Switched_By_Indexer) {
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

TEST(Database, Insert_And_Get) {
  Database<int> db;
  int data1 = rand();
  int data2 = rand();
  auto view1 = insertAndGet<int>(db, data1);
  auto view2 = insertAndGet<int>(db, data2);
  ASSERT_EQ(data1, *view1);
  ASSERT_EQ(data2, *view2);
}

TEST(Database, Call_Update_Data_On_Indexer_After_View_Scope_Ends) {
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

TEST(Database, Check_IsValid_State_With_Indexer_During_Get) {
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

TEST(Database,
     Do_Not_Check_IsValid_State_With_Not_Supported_Indexer_During_Get) {
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

TEST(Database, Handle_Exception_From_IsValid_During_Get) {
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

TEST(Database, Handle_Exception_From_Supports_Query_During_IsValid_Check) {
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

TEST(Database, Handle_Exception_From_Supports_Query_During_Get) {
  Database<int> db;
  auto indexer = Database_getIndexer<int>();
  auto query = Database_getQuery();
  EXPECT_CALL(*indexer, supportsQuery(_)).WillOnce(Throw(std::exception()));
  db.addIndexer(std::move(indexer));
  ASSERT_NO_THROW(db.get(query));
}