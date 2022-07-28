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
              (std::shared_ptr<MQuery>), (override, const));
};

template <typename T>
std::shared_ptr<NiceMock<IndexerTest_Indexer<T>>> IndexerTest_getIndexer() {
  return std::make_shared<NiceMock<IndexerTest_Indexer<T>>>();
}

TEST(Indexer, SupportsDerivedQuery) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto query = std::make_shared<DerivedMQuery>();
  ASSERT_TRUE(indexer->supportsQuery(query));
}

TEST(Indexer, NotSupportsNotDerivedQuery) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto query = std::make_shared<NotDerivedMQuery>();
  ASSERT_FALSE(indexer->supportsQuery(query));
}

TEST(Indexer, NotSupportsNullptrQuery) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto query = std::shared_ptr<DerivedMQuery>(nullptr);
  ASSERT_FALSE(indexer->supportsQuery(query));
}

TEST(Indexer, GetIndexListThrowsExceptionWhenCalledWithoutDerivedQuery) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  auto query = std::make_shared<NotDerivedMQuery>();
  EXPECT_CALL(*indexer, getIndexList(_)).Times(0);
  ASSERT_THROW(baseIndexer->getIndexList(query), DBException);
}

TEST(Indexer, CallsDerivedGetIndexList) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  auto query = std::make_shared<DerivedMQuery>();
  EXPECT_CALL(*indexer, getIndexList(_)).Times(1);
  baseIndexer->getIndexList(query);
}

TEST(Indexer, QueryStillExistsAfterGetIndexList) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  auto query = std::make_shared<DerivedMQuery>();
  int value = rand();
  query->_value = value;
  baseIndexer->getIndexList(query);
  ASSERT_EQ(value, query->_value);
}

TEST(Indexer, DefaultAddData) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  int value = rand();
  ASSERT_NO_THROW(baseIndexer->addData(value, 0));
}

TEST(Indexer, DefaultUpdateData) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  int value = rand();
  ASSERT_NO_THROW(baseIndexer->updateData(value, 0));
}

TEST(Indexer, DefaultRemoveData) {
  auto indexer = IndexerTest_getIndexer<int>();
  auto baseIndexer(std::static_pointer_cast<IIndexer<int>>(indexer));
  int value = rand();
  ASSERT_NO_THROW(baseIndexer->removeData(value, 0));
}

TEST(Indexer, ThrowsOnNullptrQuery) {
  auto indexer =
      std::static_pointer_cast<IIndexer<int>>(IndexerTest_getIndexer<int>());
  ASSERT_THROW(indexer->getIndexList(std::shared_ptr<IQuery>(nullptr)),
               Exception);
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
  Indexer_Data(int x, int y) : _name("data"), _p(Indexer_Parameter(x, y)) {}
  Indexer_Data(std::string name, int x, int y)
      : _name(name), _p(Indexer_Parameter(x, y)) {}
};

class Indexer_QRYParameter : public QRYParameter<Indexer_Parameter> {};

class Indexer_QRYOtherParameter : public QRYParameter<int> {};

class Indexer_ParameterIndexer
    : public ParameterIndexer<Indexer_Data, Indexer_Parameter> {

public:
  Indexer_ParameterIndexer() {}

  virtual Indexer_Parameter getValue(const Indexer_Data &entry) const {
    return entry._p;
  }
};

TEST(Indexer, ParameterIndexerSupportsParameterQuery) {
  Indexer_ParameterIndexer indexer;
  auto qry = std::make_shared<Indexer_QRYParameter>();
  ASSERT_TRUE(indexer.supportsQuery(qry));
}

TEST(Indexer, ParameterIndexerDoesNotSupportOtherQuery) {
  Indexer_ParameterIndexer indexer;
  auto qry = std::make_shared<MQuery>();
  ASSERT_FALSE(indexer.supportsQuery(qry));
}

TEST(Indexer, ParameterIndexerDoesNotSupportOtherParameter) {
  Indexer_ParameterIndexer indexer;
  auto qry = std::make_shared<Indexer_QRYOtherParameter>();
  ASSERT_FALSE(indexer.supportsQuery(qry));
}

TEST(Indexer, ParameterIndexerCallsDerivedIndexer) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  auto baseIndexer = std::static_pointer_cast<
      ParameterIndexer<Indexer_Data, Indexer_Parameter>>(indexer);
  auto qry = std::make_shared<Indexer_QRYParameter>();
  EXPECT_CALL(*indexer, getByParameter(Eq(qry))).Times(1);
  baseIndexer->getIndexList(qry);
}

TEST(Indexer,
     ParameterIndexerGetIndexListThrowsExceptionWhenCalledWithWrongParameter) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  auto baseIndexer = std::static_pointer_cast<
      ParameterIndexer<Indexer_Data, Indexer_Parameter>>(indexer);
  auto qry = std::make_shared<Indexer_QRYOtherParameter>();
  EXPECT_CALL(*indexer, getByParameter(_)).Times(0);
  ASSERT_THROW(baseIndexer->getIndexList(qry), Exception);
}

TEST(Indexer,
     ParameterIndexerGetIndexListThrowsExceptionWhenCalledWithWrongQuery) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  auto baseIndexer = std::static_pointer_cast<IIndexer<Indexer_Data>>(indexer);
  auto qry = std::make_shared<MQuery>();
  EXPECT_CALL(*indexer, getByParameter(_)).Times(0);
  ASSERT_THROW(baseIndexer->getIndexList(qry), Exception);
}

TEST(Indexer, ParameterIndexerCallsGetValueDuringAddData) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  Indexer_Data data;
  EXPECT_CALL(*indexer, getValue(Ref(data))).Times(1);
  indexer->addData(data, 0);
}

TEST(Indexer, ParameterIndexerCallsGetValueDuringRemoveData) {
  auto indexer =
      std::make_shared<MParameterIndexer<Indexer_Data, Indexer_Parameter>>();
  Indexer_Data data;
  EXPECT_CALL(*indexer, getValue(Ref(data))).Times(1);
  indexer->removeData(data, rand());
}

TEST(Indexer, ParameterIndexerReturnsEmptyListIfQueryDoesNotFindEntries) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  Indexer_Parameter param(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(param);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(0, result.size());
}

TEST(Indexer, ParameterIndexerAddSingleEntry) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index = rand();
  Indexer_Data data(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(index, result[0]);
}

TEST(Indexer, ParameterIndexerAddDuplicateEntry) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index1 = rand();
  auto index2 = index1 + 1;
  Indexer_Data data(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index1);
  indexer->addData(data, index2);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(index1, result[0]);
  ASSERT_EQ(index1, result[0]);
}

TEST(Indexer, ParameterIndexerThrowsWhenAddingDuplicateIndex) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index = rand();
  Indexer_Data data(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index);
  ASSERT_THROW(indexer->addData(data, index), Exception);
}

TEST(Indexer, ParameterIndexerQueryAfterEntryIsRemoved) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index = rand();
  Indexer_Data data(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index);
  indexer->removeData(data, index);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(0, result.size());
}

TEST(Indexer, ParameterIndexerUpdateWhenParameterChangedValue) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index = rand();
  Indexer_Data data(rand(), rand());
  indexer->addData(data, index);
  data._p = {rand(), rand()};
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->updateData(data, index);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(index, result[0]);
}

TEST(Indexer, ParameterIndexerCanNotFindOldEntryAfterUpdate) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  auto index = rand();
  Indexer_Data data1(rand(), rand());
  Indexer_Data data2(rand(), rand());
  indexer->addData(data1, index);
  indexer->updateData(data2, index);
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data1._p);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(0, result.size());
}

TEST(Indexer, ParameterIndexerThrowWhenUpdateNotExistingValue) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  Indexer_Data data(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  ASSERT_THROW(indexer->updateData(data, rand()), Exception);
}

TEST(Indexer, ParameterIndexerIsValidCheck) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  unsigned int index = rand();
  Indexer_Data data(rand(), rand());
  indexer->addData(data, index);
  ASSERT_TRUE(indexer->isValid(data, index));
}

TEST(Indexer, ParameterIndexerIsNotValidCheck) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  unsigned int index = rand();
  Indexer_Data data(rand(), rand());
  indexer->addData(data, index);
  data._p._x++;
  ASSERT_FALSE(indexer->isValid(data, index));
}

TEST(Indexer, ParameterIndexerIsNotValidCheckForEmptyIndex) {
  auto indexer = std::make_shared<Indexer_ParameterIndexer>();
  unsigned int index = rand();
  Indexer_Data data;
  ASSERT_FALSE(indexer->isValid(data, index));
}

TEST(Indexer, IDXIndexGetIndexList) {
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

TEST(Indexer, IDXIndexGetEmptyIndexList) {
  auto qry = std::make_shared<QRYIndex>();
  IDXIndexer<int> indexer;
  auto result = indexer.getIndexList(qry);
  ASSERT_EQ(0, result.size());
}

TEST(Indexer, IDXIndexIsValidCheck) {
  auto indexer = std::make_shared<IDXIndexer<Indexer_Data>>();
  unsigned int index = rand();
  Indexer_Data data(rand(), rand());
  indexer->addData(data, index);
  ASSERT_TRUE(indexer->isValid(data, index));
}