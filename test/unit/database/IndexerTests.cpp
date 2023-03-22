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

#include <CAMIndexer.hpp>
#include <CAMWrapper.hpp>
#include <DENMIndexer.hpp>
#include <DENMWrapper.hpp>
#include <IDXIndex.hpp>
#include <IDXParameter.hpp>
#include <Indexer.hpp>
#include <Mocks.hpp>
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

class Indexer_IDXParameter
    : public IDXParameter<Indexer_Data, Indexer_Parameter> {

public:
  Indexer_IDXParameter() {}

  virtual Indexer_Parameter getValue(const Indexer_Data &entry) const {
    return entry._p;
  }
};

TEST(Indexer, IDXParameterSupportsParameterQuery) {
  Indexer_IDXParameter indexer;
  auto qry = std::make_shared<Indexer_QRYParameter>();
  ASSERT_TRUE(indexer.supportsQuery(qry));
}

TEST(Indexer, IDXParameterDoesNotSupportOtherQuery) {
  Indexer_IDXParameter indexer;
  auto qry = std::make_shared<MQuery>();
  ASSERT_FALSE(indexer.supportsQuery(qry));
}

TEST(Indexer, IDXParameterDoesNotSupportOtherParameter) {
  Indexer_IDXParameter indexer;
  auto qry = std::make_shared<Indexer_QRYOtherParameter>();
  ASSERT_FALSE(indexer.supportsQuery(qry));
}

TEST(Indexer, IDXParameterDoesNotSupportNullQuery) {
  Indexer_IDXParameter indexer;
  auto qry = std::shared_ptr<Indexer_QRYParameter>(nullptr);
  ASSERT_FALSE(indexer.supportsQuery(qry));
}

TEST(Indexer, IDXParameterCallsDerivedIndexer) {
  auto indexer =
      std::make_shared<MIDXParameter<Indexer_Data, Indexer_Parameter>>();
  auto baseIndexer =
      std::static_pointer_cast<IDXParameter<Indexer_Data, Indexer_Parameter>>(
          indexer);
  auto qry = std::make_shared<Indexer_QRYParameter>();
  EXPECT_CALL(*indexer, getByParameter(Eq(qry))).Times(1);
  baseIndexer->getIndexList(qry);
}

TEST(Indexer,
     IDXParameterGetIndexListThrowsExceptionWhenCalledWithWrongParameter) {
  auto indexer =
      std::make_shared<MIDXParameter<Indexer_Data, Indexer_Parameter>>();
  auto baseIndexer =
      std::static_pointer_cast<IDXParameter<Indexer_Data, Indexer_Parameter>>(
          indexer);
  auto qry = std::make_shared<Indexer_QRYOtherParameter>();
  EXPECT_CALL(*indexer, getByParameter(_)).Times(0);
  ASSERT_THROW(baseIndexer->getIndexList(qry), Exception);
}

TEST(Indexer, IDXParameterGetIndexListThrowsExceptionWhenCalledWithWrongQuery) {
  auto indexer =
      std::make_shared<MIDXParameter<Indexer_Data, Indexer_Parameter>>();
  auto baseIndexer = std::static_pointer_cast<IIndexer<Indexer_Data>>(indexer);
  auto qry = std::make_shared<MQuery>();
  EXPECT_CALL(*indexer, getByParameter(_)).Times(0);
  ASSERT_THROW(baseIndexer->getIndexList(qry), Exception);
}

TEST(Indexer, IDXParameterCallsGetValueDuringAddData) {
  auto indexer =
      std::make_shared<MIDXParameter<Indexer_Data, Indexer_Parameter>>();
  Indexer_Data data;
  EXPECT_CALL(*indexer, getValue(Ref(data))).Times(1);
  indexer->addData(data, 0);
}

TEST(Indexer, IDXParameterCallsGetValueDuringRemoveData) {
  auto indexer =
      std::make_shared<MIDXParameter<Indexer_Data, Indexer_Parameter>>();
  Indexer_Data data;
  EXPECT_CALL(*indexer, getValue(Ref(data))).Times(1);
  indexer->removeData(data, rand());
}

TEST(Indexer, IDXParameterReturnsEmptyListIfQueryDoesNotFindEntries) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
  Indexer_Parameter param(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(param);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(0, result.size());
}

TEST(Indexer, IDXParameterAddSingleEntry) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
  auto index = rand();
  Indexer_Data data(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(1, result.size());
  ASSERT_EQ(index, result[0]);
}

TEST(Indexer, IDXParameterAddDuplicateEntry) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
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

TEST(Indexer, IDXParameterThrowsWhenAddingDuplicateIndex) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
  auto index = rand();
  Indexer_Data data(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index);
  ASSERT_THROW(indexer->addData(data, index), Exception);
}

TEST(Indexer, IDXParameterQueryAfterEntryIsRemoved) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
  auto index = rand();
  Indexer_Data data(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  indexer->addData(data, index);
  indexer->removeData(data, index);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(0, result.size());
}

TEST(Indexer, IDXParameterUpdateWhenParameterChangedValue) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
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

TEST(Indexer, IDXParameterCanNotFindOldEntryAfterUpdate) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
  auto index = rand();
  Indexer_Data data1(rand(), rand());
  Indexer_Data data2(rand(), rand());
  indexer->addData(data1, index);
  indexer->updateData(data2, index);
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data1._p);
  auto result = indexer->getIndexList(qry);
  ASSERT_EQ(0, result.size());
}

TEST(Indexer, IDXParameterThrowWhenUpdateNotExistingValue) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
  Indexer_Data data(rand(), rand());
  auto qry = std::make_shared<QRYParameter<Indexer_Parameter>>(data._p);
  ASSERT_THROW(indexer->updateData(data, rand()), Exception);
}

TEST(Indexer, IDXParameterIsValidCheck) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
  unsigned int index = rand();
  Indexer_Data data(rand(), rand());
  indexer->addData(data, index);
  ASSERT_TRUE(indexer->isValid(data, index));
}

TEST(Indexer, IDXParameterIsNotValidCheck) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
  unsigned int index = rand();
  Indexer_Data data(rand(), rand());
  indexer->addData(data, index);
  data._p._x++;
  ASSERT_FALSE(indexer->isValid(data, index));
}

TEST(Indexer, IDXParameterIsNotValidCheckForEmptyIndex) {
  auto indexer = std::make_shared<Indexer_IDXParameter>();
  unsigned int index = rand();
  Indexer_Data data;
  ASSERT_FALSE(indexer->isValid(data, index));
}

TEST(Indexer, IDXIndexGetIndexList) {
  auto index = (unsigned int)(rand() % 10000) + 100;
  auto len = rand() % 100;
  auto qry = std::make_shared<QRYIndex>(index, index + len);
  IDXIndex<int> indexer;
  auto result = indexer.getIndexList(qry);
  ASSERT_EQ(len, result.size());
  for (unsigned int i = index; i < index + len; i++) {
    ASSERT_EQ(i, result[i - index]);
  }
}

TEST(Indexer, IDXIndexGetEmptyIndexList) {
  auto qry = std::make_shared<QRYIndex>();
  IDXIndex<int> indexer;
  auto result = indexer.getIndexList(qry);
  ASSERT_EQ(0, result.size());
}

TEST(Indexer, IDXIndexIsValidCheck) {
  auto indexer = std::make_shared<IDXIndex<Indexer_Data>>();
  unsigned int index = rand();
  Indexer_Data data(rand(), rand());
  indexer->addData(data, index);
  ASSERT_TRUE(indexer->isValid(data, index));
}

TEST(Indexer, IDXCAMStationID) {
  auto stationID = (unsigned int)rand();
  cam::CAMWrapper cam(stationID);
  cam::IDXCAMStationID indexer;
  ASSERT_EQ(stationID, indexer.getValue(cam));
}

TEST(Indexer, IDXDENMStationID) {
  auto stationID = (unsigned int)rand();
  denm::DENMWrapper denm(stationID, 0);
  denm::IDXDENMStationID indexer;
  ASSERT_EQ(stationID, indexer.getValue(denm));
}

TEST(Indexer, IDXCAMLatestGetIndex) {
  auto stationID = (unsigned int)rand();
  auto index = (unsigned int)rand();

  cam::CAMWrapper cam(stationID);
  cam::IDXCAMLatest indexer;

  indexer.addData(cam, index);
  auto indexList = indexer.getIndexList(QRYLatestMsg::byId(stationID));

  ASSERT_EQ(1, indexList.size());
  ASSERT_EQ(index, indexList.at(0));
}

TEST(Indexer, IDXCAMLatestOverrideLatestMessage) {
  auto stationID = (unsigned int)rand();
  auto index1 = (unsigned int)rand();
  auto index2 = (unsigned int)rand();

  cam::CAMWrapper cam1(stationID);
  cam::CAMWrapper cam2(stationID);
  cam::IDXCAMLatest indexer;

  indexer.addData(cam1, index1);
  indexer.addData(cam2, index2);
  auto indexList = indexer.getIndexList(QRYLatestMsg::byId(stationID));

  ASSERT_EQ(1, indexList.size());
  ASSERT_EQ(index2, indexList.at(0));
}

TEST(Indexer, IDXCAMLatestRemoveMessage) {
  auto stationID = (unsigned int)rand();
  auto index = (unsigned int)rand();

  cam::CAMWrapper cam(stationID);
  cam::IDXCAMLatest indexer;

  indexer.addData(cam, index);
  indexer.removeData(cam, index);
  auto indexList = indexer.getIndexList(QRYLatestMsg::byId(stationID));

  ASSERT_EQ(0, indexList.size());
}

TEST(Indexer, Indexer_IDXCAMLatestRemoveOldMessageOnUpdateMessage) {
  auto stationID1 = (unsigned int)rand();
  auto stationID2 = stationID1 + 1;
  auto index = (unsigned int)rand();

  cam::CAMWrapper cam(stationID1);
  cam::IDXCAMLatest indexer;

  indexer.addData(cam, index);
  cam->header.stationID = stationID2;
  indexer.updateData(cam, index);
  auto indexList1 = indexer.getIndexList(QRYLatestMsg::byId(stationID1));
  auto indexList2 = indexer.getIndexList(QRYLatestMsg::byId(stationID2));

  ASSERT_EQ(0, indexList1.size());
  ASSERT_EQ(1, indexList2.size());
  ASSERT_EQ(index, indexList2.at(0));
}

TEST(Indexer, Indexer_IDXCAMLatestOverrideLatestOnUpdateMessage) {
  auto stationID = (unsigned int)rand();
  auto index1 = (unsigned int)rand();
  auto index2 = (unsigned int)rand();

  cam::CAMWrapper cam1(stationID);
  cam::CAMWrapper cam2(stationID);
  cam::IDXCAMLatest indexer;

  indexer.addData(cam1, index1);
  indexer.addData(cam2, index2);
  indexer.updateData(cam1, index1);
  auto indexList = indexer.getIndexList(QRYLatestMsg::byId(stationID));

  ASSERT_EQ(1, indexList.size());
  ASSERT_EQ(index1, indexList.at(0));
}

TEST(Indexer, IDXCAMLatestRemoveOtherMessage) {
  auto stationID = (unsigned int)rand();
  auto index1 = (unsigned int)rand();
  auto index2 = (unsigned int)rand();

  cam::CAMWrapper cam1(stationID);
  cam::CAMWrapper cam2(stationID);
  cam::IDXCAMLatest indexer;

  indexer.addData(cam1, index1);
  indexer.addData(cam2, index2);
  indexer.removeData(cam1, index1);
  auto indexList = indexer.getIndexList(QRYLatestMsg::byId(stationID));

  ASSERT_EQ(1, indexList.size());
  ASSERT_EQ(index2, indexList.at(0));
}

TEST(Indexer, IDXCAMLatestUpdateWithInvalidIndex) {
  cam::CAMWrapper cam;
  cam::IDXCAMLatest indexer;

  ASSERT_NO_THROW(indexer.updateData(cam, rand()));
}

TEST(Indexer, IDXCAMLatestAddWithInvalidIndex) {
  cam::CAMWrapper cam1;
  cam::CAMWrapper cam2;
  cam::IDXCAMLatest indexer;
  auto index = (unsigned int)rand();

  indexer.addData(cam1, index);
  ASSERT_THROW(indexer.addData(cam2, index), Exception);
}

TEST(Indexer, IDXCAMLatestRemoveWithInvalidIndex) {
  cam::CAMWrapper cam;
  cam::IDXCAMLatest indexer;

  ASSERT_NO_THROW(indexer.removeData(cam, 0));
}

TEST(Indexer, IDXCAMLatestIsValid) {
  auto stationID = (unsigned int)rand();
  auto index = (unsigned int)rand();

  cam::CAMWrapper cam(stationID);
  cam::IDXCAMLatest indexer;

  indexer.addData(cam, index);
  ASSERT_TRUE(indexer.isValid(cam, index));
}

TEST(Indexer, IDXCAMLatestIsNotValidByDifferentCAM) {
  auto stationID1 = (unsigned int)rand();
  auto stationID2 = (unsigned int)rand();
  auto index = (unsigned int)rand();

  cam::CAMWrapper cam1(stationID1);
  cam::CAMWrapper cam2(stationID2);
  cam::IDXCAMLatest indexer;

  indexer.addData(cam1, index);
  ASSERT_FALSE(indexer.isValid(cam2, index));
}

TEST(Indexer, IDXCAMLatestIsNotValidByDifferentIndex) {
  auto stationID = (unsigned int)rand();
  auto index = (unsigned int)rand();

  cam::CAMWrapper cam(stationID);
  cam::IDXCAMLatest indexer;

  ASSERT_FALSE(indexer.isValid(cam, index));
}