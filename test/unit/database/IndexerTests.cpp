#include <IDXIndex.hpp>
#include <Indexer.hpp>
#include <Mocks.hpp>
#include <ParameterIndexer.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

using ::testing::_;
using ::testing::Eq;
using ::testing::NiceMock;
using ::testing::Ref;

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

struct Indexer_Parameter {
  int _x;
  int _y;

  Indexer_Parameter() : Indexer_Parameter(0, 0) {}
  Indexer_Parameter(int x, int y) : _x(x), _y(y) {}
};

template <> struct std::hash<Indexer_Parameter> {
  std::size_t operator()(const Indexer_Parameter &p) const {
    return (std::hash<int>()(p._x) ^ (std::hash<int>()(p._y) << 1));
  }
};

bool operator==(const Indexer_Parameter &p1, const Indexer_Parameter &p2) {
  return p1._x == p2._x && p1._y == p2._y;
}

struct Indexer_Data {
  std::string _name;
  Indexer_Parameter _p;

  Indexer_Data() {}
  Indexer_Data(std::string name, int x, int y)
      : _name(name), _p(Indexer_Parameter(x, y)) {}
};

class Indexer_QRYParameter : public QRYParameter<Indexer_Parameter> {};

class Indexer_QRYOtherParameter : public QRYParameter<int> {};

class Indexer_ParameterIndexer
    : public ParameterIndexer<Indexer_Data, Indexer_Parameter> {

public:
  Indexer_ParameterIndexer() {}

  virtual Indexer_Parameter getValue(const Indexer_Data &entry) {
    return entry._p;
  }
};

TEST(Indexer, ParameterIndexer_Supports_Parameter_Query) {
  Indexer_ParameterIndexer indexer;
  auto qry = std::make_shared<Indexer_QRYParameter>();
  ASSERT_TRUE(indexer.supportsQuery(qry));
}

TEST(Indexer, ParameterIndexer_Does_Not_Support_Other_Query) {
  Indexer_ParameterIndexer indexer;
  auto qry = std::make_shared<MQuery>();
  ASSERT_FALSE(indexer.supportsQuery(qry));
}

TEST(Indexer, ParameterIndexer_Does_Not_Support_Other_Parameter) {
  Indexer_ParameterIndexer indexer;
  auto qry = std::make_shared<Indexer_QRYOtherParameter>();
  ASSERT_FALSE(indexer.supportsQuery(qry));
}

TEST(Indexer, ParameterIndexer_Calls_Derived_Indexer) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  auto baseIndexer = std::static_pointer_cast<
      ParameterIndexer<Indexer_Data, Indexer_Parameter>>(indexer);
  auto qry = std::make_shared<Indexer_QRYParameter>();
  EXPECT_CALL(*indexer, getByParameter(Eq(qry))).Times(1);
  baseIndexer->getIndexList(qry);
}

TEST(
    Indexer,
    ParameterIndexer_GetIndexList_Throws_Exception_When_Called_With_Wrong_Parameter) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  auto baseIndexer = std::static_pointer_cast<
      ParameterIndexer<Indexer_Data, Indexer_Parameter>>(indexer);
  auto qry = std::make_shared<Indexer_QRYOtherParameter>();
  EXPECT_CALL(*indexer, getByParameter(_)).Times(0);
  ASSERT_THROW(baseIndexer->getIndexList(qry), Exception);
}

TEST(
    Indexer,
    ParameterIndexer_GetIndexList_Throws_Exception_When_Called_With_Wrong_Query) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  auto baseIndexer = std::static_pointer_cast<IIndexer<Indexer_Data>>(indexer);
  auto qry = std::make_shared<MQuery>();
  EXPECT_CALL(*indexer, getByParameter(_)).Times(0);
  ASSERT_THROW(baseIndexer->getIndexList(qry), Exception);
}

TEST(Indexer, ParameterIndexer_Calls_GetValue_During_AddData) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  Indexer_Data data;
  EXPECT_CALL(*indexer, getValue(Ref(data))).Times(1);
  indexer->addData(data, 0);
}

TEST(Indexer, ParameterIndexer_Calls_GetValue_During_RemoveData) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  Indexer_Data data;
  EXPECT_CALL(*indexer, getValue(Ref(data))).Times(1);
  indexer->removeData(data, rand());
}

TEST(Indexer,
     ParameterIndexer_Returns_Empty_List_If_Query_Does_Not_Find_Entries) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  Indexer_Parameter param(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(param);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(0, result.size());
}

TEST(Indexer, ParameterIndexer_Add_Single_Entry) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index = rand();
  Indexer_Data data("data1", rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(index, result[0]);
}

TEST(Indexer, ParameterIndexer_Add_Duplicate_Entry) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index1 = rand();
  auto index2 = index1 + 1;
  Indexer_Data data("data1", rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index1);
  indexer->addData(data, index2);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(index1, result[0]);
  ASSERT_EQ(index1, result[0]);
}

TEST(Indexer, ParameterIndexer_Throws_When_Adding_Duplicate_Index) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index = rand();
  Indexer_Data data("data1", rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index);
  ASSERT_THROW(indexer->addData(data, index), Exception);
}

TEST(Indexer, ParameterIndexer_Query_After_Entry_Is_Removed) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index = rand();
  Indexer_Data data("data1", rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index);
  indexer->removeData(data, index);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(0, result.size());
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