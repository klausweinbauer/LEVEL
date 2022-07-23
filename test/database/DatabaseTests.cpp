#include <Database.hpp>
#include <Indexer.hpp>
#include <Mocks.hpp>
#include <Query.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

using ::testing::_;
using ::testing::Eq;
using ::testing::Ref;
using ::testing::Return;

TEST(Database_Database, Test_Create) {
  Database<int> db;
  int x = rand();

  auto view = db.insert(&x);

  ASSERT_EQ(x, *view);
}

TEST(Database_Database, Test_Count_Empty_Database) {
  Database<int> db;
  ASSERT_EQ(0, db.count());
}

TEST(Database_Database, Test_Count) {
  Database<int> db;
  int x = rand();
  db.insert(&x);
  db.insert(&x);
  ASSERT_EQ(2, db.count());
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
  int entry = rand();
  db.insert(&entry);
  std::vector<unsigned int> indexerReturn = {0};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query)))
      .WillRepeatedly(Return(indexerReturn));

  auto ret = db.get(query);

  ASSERT_EQ(1, ret.size());
  ASSERT_EQ(entry, *ret[0]);
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
  int entry = rand();
  db.insert(&entry);
  std::vector<unsigned int> indexerReturn = {0};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query)))
      .WillRepeatedly(Return(indexerReturn));

  auto ret = db.get(query);

  ASSERT_EQ(1, ret.size());
  ASSERT_EQ(entry, *ret[0]);
}

TEST(Database_Database, Test_Get_Multiple_Results) {
  int queryType = rand();
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  Database<int> db;
  db.addIndexer(indexer);
  Query query(queryType);
  int entry1 = rand();
  int entry2 = rand();
  db.insert(&entry1);
  db.insert(&entry2);
  std::vector<unsigned int> indexerReturn = {0, 1};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query)))
      .WillRepeatedly(Return(indexerReturn));

  auto ret = db.get(query);

  ASSERT_EQ(2, ret.size());
  ASSERT_EQ(entry1, *ret[0]);
  ASSERT_EQ(entry2, *ret[1]);
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
  int entry1 = rand();
  int entry2 = rand();
  db.insert(&entry1);
  db.insert(&entry2);
  std::vector<unsigned int> indexerReturn = {0, 1};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(Ref(query)))
      .WillRepeatedly(Return(indexerReturn));

  auto ret = db.get(query);

  ASSERT_EQ(2, ret.size());
  ASSERT_EQ(entry1, *ret[0]);
  ASSERT_EQ(entry2, *ret[1]);
}

/*TEST(Database_Database, Test_Delete_Entry) {
  int queryType = rand();
  Database<int> db;
  std::shared_ptr<MIndexer<int>> indexer(new MIndexer<int>());
  db.addIndexer(indexer);
  Query query(queryType);
  std::vector<unsigned int> indexerReturn = {0};

  EXPECT_CALL(*indexer, getQueryType()).WillRepeatedly(Return(queryType));
  EXPECT_CALL(*indexer, getIndexList(_)).WillOnce(Return(indexerReturn));

  db.remove(query);

  ASSERT_EQ(0, db.count());
}*/