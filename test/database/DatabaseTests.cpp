#include <Database.hpp>
#include <Indexer.hpp>
#include <Mocks.hpp>
#include <Query.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

using ::testing::_;
using ::testing::Eq;
using ::testing::Invoke;
using ::testing::Ref;
using ::testing::Return;
using ::testing::Throw;

TEST(Database_Database, Test_Create) {
  Database<int> db;
  int *x = new int(rand());

  auto view = db.insert(x);

  ASSERT_EQ(*x, *view);
}

TEST(Database_Database, Test_Count_Empty_Database) {
  Database<int> db;
  ASSERT_EQ(0, db.count());
}

TEST(Database_Database, Test_Count) {
  Database<int> db;
  int *x1 = new int(rand());
  int *x2 = new int(rand());
  db.insert(x1);
  db.insert(x2);
  ASSERT_EQ(2, db.count());
}

TEST(Database_Database, Test_Error_On_Duplicate_Insert) {
  Database<int> db;
  int *x = new int(rand());
  db.insert(x);
  bool threwException = false;
  try {
    db.insert(x);
  } catch (const DBException &e) {
    threwException = true;
  }

  ASSERT_TRUE(threwException);
}

TEST(Database_Database, Test_Insert_Nullptr) {
  Database<int> db;
  bool threwException = false;
  try {
    db.insert(nullptr);
  } catch (const DBException &) {
    threwException = true;
  }

  ASSERT_TRUE(threwException);
}

TEST(Database_Database, Test_Call_To_Indexer_On_Get) {
  int queryType = rand();
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  Database<int> db;
  db.addIndexer(indexer);
  Query query(queryType);

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query))).Times(1);

  db.get(query);
}

TEST(Database_Database, Test_No_Call_To_Incorrect_Indexer_On_Get) {
  int queryType = rand();
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  Database<int> db;
  db.addIndexer(indexer);
  Query query(queryType);

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType + 1));
  EXPECT_CALL(*indexer, getIndexList(_)).Times(0);

  db.get(query);
}

TEST(Database_Database, Test_Get_Return_Value) {
  int queryType = rand();
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  Database<int> db;
  db.addIndexer(indexer);
  Query query(queryType);
  int *entry = new int(rand());
  db.insert(entry);
  std::vector<unsigned int> indexerReturn = {0};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query)))
      .WillRepeatedly(Return(indexerReturn));

  auto ret = db.get(query);

  ASSERT_EQ(1, ret.size());
  ASSERT_EQ(*entry, *ret[0]);
}

TEST(Database_Database,
     Test_Get_Single_Return_Value_From_Multiple_Indexer_Results) {
  int queryType = rand();
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  Database<int> db;
  db.addIndexer(indexer);
  db.addIndexer(indexer);
  db.addIndexer(indexer);
  Query query(queryType);
  int *entry = new int(rand());
  db.insert(entry);
  std::vector<unsigned int> indexerReturn = {0};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query)))
      .WillRepeatedly(Return(indexerReturn));

  auto ret = db.get(query);

  ASSERT_EQ(1, ret.size());
  ASSERT_EQ(*entry, *ret[0]);
}

TEST(Database_Database, Test_Get_Multiple_Results) {
  int queryType = rand();
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  Database<int> db;
  db.addIndexer(indexer);
  Query query(queryType);
  int *entry1 = new int(rand());
  int *entry2 = new int(rand());
  db.insert(entry1);
  db.insert(entry2);
  std::vector<unsigned int> indexerReturn = {0, 1};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query)))
      .WillRepeatedly(Return(indexerReturn));

  auto ret = db.get(query);

  ASSERT_EQ(2, ret.size());
  ASSERT_EQ(*entry1, *ret[0]);
  ASSERT_EQ(*entry2, *ret[1]);
}

TEST(Database_Database,
     Test_Get_Multiple_Results_From_Multiple_Indexer_Returns) {
  int queryType = rand();
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  Database<int> db;
  db.addIndexer(indexer);
  db.addIndexer(indexer);
  db.addIndexer(indexer);
  Query query(queryType);
  int *entry1 = new int(rand());
  int *entry2 = new int(rand());
  db.insert(entry1);
  db.insert(entry2);
  std::vector<unsigned int> indexerReturn = {0, 1};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query)))
      .WillRepeatedly(Return(indexerReturn));

  auto ret = db.get(query);

  ASSERT_EQ(2, ret.size());
  ASSERT_EQ(*entry1, *ret[0]);
  ASSERT_EQ(*entry2, *ret[1]);
}

TEST(Database_Database, Test_Indexer_Returns_Invalid_Index) {
  int queryType = rand();
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  Database<int> db;
  db.addIndexer(indexer);
  Query query(queryType);
  std::vector<unsigned int> indexerReturn = {1};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query)))
      .WillRepeatedly(Return(indexerReturn));

  auto ret = db.get(query);

  ASSERT_EQ(0, ret.size());
}

TEST(Database_Database, Test_Delete_Entry) {
  int queryType = rand();
  Database<int> db;
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  db.addIndexer(indexer);
  Query query(queryType);
  int *entry = new int(rand());
  db.insert(entry);
  std::vector<unsigned int> indexerReturn = {0};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(_)).WillOnce(Return(indexerReturn));

  int ret = db.remove(query);

  ASSERT_EQ(1, ret);
  ASSERT_EQ(0, db.count());
}

TEST(Database_Database, Test_Reuse_Of_Deleted_Index) {
  int queryType = rand();
  Database<int> db;
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  db.addIndexer(indexer);
  Query query(queryType);
  int *entry1 = new int(rand());
  int *entry2 = new int(rand());
  db.insert(entry1);
  std::vector<unsigned int> indexerReturn = {0};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(_)).WillRepeatedly(Return(indexerReturn));

  db.remove(query);
  auto view = db.insert(entry2);

  ASSERT_EQ(1, db.count());
  ASSERT_EQ(*entry2, *view);
}

TEST(Database_Database, Test_Indexer_Throws_Exception_In_Get) {
  int queryType = rand();
  Database<int> db;
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  db.addIndexer(indexer);
  Query query(queryType);
  bool threwException = true;

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(_))
      .WillRepeatedly(Throw(std::exception()));

  try {
    auto res = db.get(query);
    threwException = false;
    ASSERT_EQ(0, res.size());
  } catch (const std::exception &e) {
  }

  ASSERT_FALSE(threwException);
}

TEST(Database_Database, Test_Indexer_Throws_Exception_In_Remove) {
  int queryType = rand();
  Database<int> db;
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  db.addIndexer(indexer);
  Query query(queryType);
  bool threwException = true;

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(_))
      .WillRepeatedly(Throw(std::exception()));

  try {
    auto res = db.remove(query);
    threwException = false;
    ASSERT_EQ(0, res);
  } catch (const std::exception &e) {
  }

  ASSERT_FALSE(threwException);
}

static int Test_Notify_Indexer_On_Value_Update_value = 0;
static void
Test_Notify_Indexer_On_Value_Update_saveValueUpdate(const int *const entry) {
  Test_Notify_Indexer_On_Value_Update_value = *entry;
}

TEST(Database_Database, Test_Notify_Indexer_On_Value_Update) {
  int queryType = rand();
  Database<int> db;
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  db.addIndexer(indexer);
  Query query(queryType);
  int *entry = new int(rand());
  int baseValue = *entry;
  db.insert(entry);
  std::vector<unsigned int> indexerReturn = {0};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(_)).WillRepeatedly(Return(indexerReturn));
  EXPECT_CALL(*indexer, valueChanged(_))
      .WillOnce(Invoke(Test_Notify_Indexer_On_Value_Update_saveValueUpdate));

  {
    auto view = db.get(query);
    *&(view[0]) += 1;
  }

  ASSERT_EQ(baseValue + 1, Test_Notify_Indexer_On_Value_Update_value);
}

TEST(Database_Database, Test_Indexer_Throws_Exception_On_Value_Update) {
  int queryType = rand();
  Database<int> db;
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  db.addIndexer(indexer);
  Query query(queryType);
  int *entry = new int(rand());
  db.insert(entry);
  std::vector<unsigned int> indexerReturn = {0};
  bool threwException = false;

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(_)).WillRepeatedly(Return(indexerReturn));
  EXPECT_CALL(*indexer, valueChanged(_)).WillOnce(Throw(std::exception()));

  try {
    auto view = db.get(query);
    *&(view[0]) += 1;
  } catch (const std::exception &) {
    threwException = true;
  }

  ASSERT_FALSE(threwException);
}