#include <IDXIndex.hpp>
#include <Indexer.hpp>
#include <Mocks.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

using ::testing::_;
using ::testing::Eq;
using ::testing::NiceMock;

class DerivedMQuery : public MQuery {
public:
  int _value;
};

class NotDerivedMQuery : public IQuery {};

template <typename T> class IndexerTest_Indexer : public Indexer<T, MQuery> {
public:
  MOCK_METHOD(std::vector<unsigned int>, getIndexList,
              (std::shared_ptr<MQuery>));
};

template <typename T>
std::shared_ptr<NiceMock<IndexerTest_Indexer<T>>> IndexerTest_getIndexer() {
  return std::make_shared<NiceMock<IndexerTest_Indexer<T>>>();
}

TEST(Indexer, Supports_Derived_Query) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto query = std::make_shared<DerivedMQuery>();
  ASSERT_TRUE(indexer->supportsQuery(query));
}

TEST(Indexer, Not_Supports_Not_Derived_Query) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto query = std::make_shared<NotDerivedMQuery>();
  ASSERT_FALSE(indexer->supportsQuery(query));
}

TEST(Indexer, Not_Supports_Nullptr_To_Query) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto query = std::shared_ptr<DerivedMQuery>(nullptr);
  ASSERT_FALSE(indexer->supportsQuery(query));
}

TEST(Indexer, GetIndexList_Throws_Exception_When_Called_Without_Derived_Query) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  auto query = std::make_shared<NotDerivedMQuery>();
  EXPECT_CALL(*indexer, getIndexList(_)).Times(0);
  ASSERT_THROW(baseIndexer->getIndexList(query), DBException);
}

TEST(Indexer, Calls_Derived_GetIndexList) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  auto query = std::make_shared<DerivedMQuery>();
  EXPECT_CALL(*indexer, getIndexList(_)).Times(1);
  baseIndexer->getIndexList(query);
}

TEST(Indexer, Query_Still_Exists_After_GetIndexList) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  auto query = std::make_shared<DerivedMQuery>();
  int value = rand();
  query->_value = value;
  baseIndexer->getIndexList(query);
  ASSERT_EQ(value, query->_value);
}

TEST(Indexer, Default_AddData) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  int value = rand();
  ASSERT_NO_THROW(baseIndexer->addData(value, 0));
}

TEST(Indexer, Default_RemoveData) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  int value = rand();
  ASSERT_NO_THROW(baseIndexer->removeData(value, 0));
}

TEST(Indexer, IDXIndex_Get_Index_List) {
  auto index = (unsigned int)(rand() % 10000) + 100;
  auto len = rand() % 100;
  auto qry = std::make_shared<QRYIndex>(index, index + len);
  IDXIndexer<int> indexer;
  auto result = indexer.getIndexList(qry);
  ASSERT_EQ(len, result.size());
  for (unsigned int i = index; i < index + len; i++) {
    ASSERT_EQ(i, result[i - index]);
  }
}

TEST(Indexer, IDXIndex_Get_Empty_Index_List) {
  auto qry = std::make_shared<QRYIndex>();
  IDXIndexer<int> indexer;
  auto result = indexer.getIndexList(qry);
  ASSERT_EQ(0, result.size());
}