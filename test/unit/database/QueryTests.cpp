#include <Exception.hpp>
#include <QRYIndex.hpp>
#include <QRYParameter.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

TEST(Query, QRYIndexDefaultConstruction) {
  QRYIndex qry;
  ASSERT_EQ(0, qry.firstIndex());
  ASSERT_EQ(0, qry.lastIndex());
  ASSERT_EQ(0, qry.length());
}

TEST(Query, QRYIndexIndexConstruction) {
  auto index = (unsigned int)rand();
  QRYIndex qry(index);
  ASSERT_EQ(index, qry.firstIndex());
  ASSERT_EQ(index + 1, qry.lastIndex());
  ASSERT_EQ(1, qry.length());
}

TEST(Query, QRYIndexRangeConstruction) {
  auto index = (unsigned int)rand();
  auto len = rand() % 100;
  QRYIndex qry(index, index + len);
  ASSERT_EQ(index, qry.firstIndex());
  ASSERT_EQ(index + len, qry.lastIndex());
  ASSERT_EQ(len, qry.length());
}

TEST(Query, QRYIndexInvalidRangeConstruction) {
  auto index = (unsigned int)rand();
  ASSERT_THROW(QRYIndex qry(index + 1, index), Exception);
}

TEST(Query, QRYIndexSetFirstIndex) {
  auto index = (unsigned int)rand();
  QRYIndex qry(0, index + 1);
  qry.setFirstIndex(index);
  ASSERT_EQ(index, qry.firstIndex());
}

TEST(Query, QRYIndexSetFirstIndexHigherThanLastIndex) {
  auto index = (unsigned int)rand();
  QRYIndex qry(0, index - 1);
  ASSERT_THROW(qry.setFirstIndex(index), Exception);
}

TEST(Query, QRYIndexSetLastIndex) {
  auto index = (unsigned int)rand();
  QRYIndex qry;
  qry.setLastIndex(index);
  ASSERT_EQ(index, qry.lastIndex());
}

TEST(Query, QRYIndexSetLastIndexSmallerThanFirstIndex) {
  auto index = (unsigned int)rand();
  QRYIndex qry(index + 1, index + 2);
  ASSERT_THROW(qry.setLastIndex(index), Exception);
}

TEST(Query, QRYIndexSetSingleIndex) {
  auto index = (unsigned int)rand();
  QRYIndex qry;
  qry.setIndex(index);
  ASSERT_EQ(index, qry.firstIndex());
  ASSERT_EQ(index + 1, qry.lastIndex());
}

TEST(Query, QRYIndexSetSingleIndexOverflow) {
  auto index = UINT32_MAX;
  QRYIndex qry;
  ASSERT_THROW(qry.setIndex(index), Exception);
}

TEST(Query, QRYIndexSetRange) {
  auto index = (unsigned int)rand();
  auto len = rand() % 100;
  QRYIndex qry;
  qry.setRange(index, len);
  ASSERT_EQ(index, qry.firstIndex());
  ASSERT_EQ(index + len, qry.lastIndex());
  ASSERT_EQ(len, qry.length());
}

TEST(Query, QRYIndexSetRangeBackwards) {
  int index = (rand() % 10000) + 1000;
  int len = rand() % 1000;
  QRYIndex qry;
  qry.setRange(index, -len);
  ASSERT_EQ(index - len + 1, qry.firstIndex());
  ASSERT_EQ(index + 1, qry.lastIndex());
  ASSERT_EQ(len, qry.length());
}

TEST(Query, QRYIndexSetRangeWithMaxPossibleValue) {
  unsigned int index = UINT32_MAX - 1;
  int len = 100;
  QRYIndex qry;
  qry.setRange(index, -len);
  ASSERT_EQ(UINT32_MAX - len, qry.firstIndex());
  ASSERT_EQ(UINT32_MAX, qry.lastIndex());
  ASSERT_EQ(len, qry.length());
}

TEST(Query, QRYIndexSetRangeWithOverflowingValue) {
  unsigned int index = UINT32_MAX;
  QRYIndex qry;
  ASSERT_THROW(qry.setRange(index, 1), Exception);
}

TEST(Query, QRYIndexSetRangeWithUnderflowingValue) {
  unsigned int index = 0;
  QRYIndex qry;
  ASSERT_THROW(qry.setRange(index, -2), Exception);
}

TEST(Query, QRYIndexByRange) {
  unsigned int index1 = (unsigned int)(rand() % 1000) + 100;
  unsigned int index2 = index1 + (rand() % 100);
  auto qry = QRYIndex::byRange(index1, index2);
  QRYIndex expQry(index1, index2);
  ASSERT_EQ(expQry.firstIndex(), qry->firstIndex());
  ASSERT_EQ(expQry.lastIndex(), qry->lastIndex());
}

TEST(Query, QRYIndexByRangeWithLength) {
  unsigned int index = (unsigned int)(rand() % 1000) + 100;
  int len = rand() % 100;
  auto qry = QRYIndex::byRange(index, len);
  QRYIndex expQry(index, len);
  ASSERT_EQ(expQry.firstIndex(), qry->firstIndex());
  ASSERT_EQ(expQry.lastIndex(), qry->lastIndex());
}

TEST(Query, QRYIndexByIndex) {
  unsigned int index = (unsigned int)(rand() % 1000) + 100;
  auto qry = QRYIndex::byIndex(index);
  ASSERT_EQ(index, qry->firstIndex());
  ASSERT_EQ(index + 1, qry->lastIndex());
}

struct QueryTest_Parameter {
  int _x;
  int _y;

  QueryTest_Parameter() : _x(0), _y(0) {}
  QueryTest_Parameter(int x, int y) : _x(x), _y(y) {}
};

TEST(Query, QRYParameterDefaultConstructor) {
  QRYParameter<QueryTest_Parameter> qry;
  ASSERT_EQ(0, qry.value()._x);
  ASSERT_EQ(0, qry.value()._y);
}

TEST(Query, QRYParameterConstructor) {
  QueryTest_Parameter param(rand(), rand());
  QRYParameter<QueryTest_Parameter> qry(param);
  ASSERT_EQ(param._x, qry.value()._x);
  ASSERT_EQ(param._y, qry.value()._y);
}

TEST(Query, QRYParameterSetValue) {
  QueryTest_Parameter param(rand(), rand());
  QRYParameter<QueryTest_Parameter> qry;
  qry.setValue(param);
  ASSERT_EQ(param._x, qry.value()._x);
  ASSERT_EQ(param._y, qry.value()._y);
}

TEST(Query, QRYParameterByValue) {
  QueryTest_Parameter param(rand(), rand());
  auto qry = QRYParameter<QueryTest_Parameter>::byValue(param);
  ASSERT_EQ(param._x, qry.value()._x);
  ASSERT_EQ(param._y, qry.value()._y);
}