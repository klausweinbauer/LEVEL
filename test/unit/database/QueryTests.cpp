#include <Exception.hpp>
#include <QRYIndex.hpp>
#include <QRYParameterValue.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;

TEST(Query, QRYIndex_Default_Construction) {
  QRYIndex qry;
  ASSERT_EQ(0, qry.firstIndex());
  ASSERT_EQ(0, qry.lastIndex());
  ASSERT_EQ(0, qry.length());
}

TEST(Query, QRYIndex_Index_Construction) {
  auto index = (unsigned int)rand();
  QRYIndex qry(index);
  ASSERT_EQ(index, qry.firstIndex());
  ASSERT_EQ(index + 1, qry.lastIndex());
  ASSERT_EQ(1, qry.length());
}

TEST(Query, QRYIndex_Range_Construction) {
  auto index = (unsigned int)rand();
  auto len = rand() % 100;
  QRYIndex qry(index, index + len);
  ASSERT_EQ(index, qry.firstIndex());
  ASSERT_EQ(index + len, qry.lastIndex());
  ASSERT_EQ(len, qry.length());
}

TEST(Query, QRYIndex_Invalid_Range_Construction) {
  auto index = (unsigned int)rand();
  ASSERT_THROW(QRYIndex qry(index + 1, index), Exception);
}

TEST(Query, QRYIndex_Set_First_Index) {
  auto index = (unsigned int)rand();
  QRYIndex qry(0, index + 1);
  qry.setFirstIndex(index);
  ASSERT_EQ(index, qry.firstIndex());
}

TEST(Query, QRYIndex_Set_First_Index_Higher_Than_Last_Index) {
  auto index = (unsigned int)rand();
  QRYIndex qry(0, index - 1);
  ASSERT_THROW(qry.setFirstIndex(index), Exception);
}

TEST(Query, QRYIndex_Set_Last_Index) {
  auto index = (unsigned int)rand();
  QRYIndex qry;
  qry.setLastIndex(index);
  ASSERT_EQ(index, qry.lastIndex());
}

TEST(Query, QRYIndex_Set_Last_Index_Smaller_Than_First_Index) {
  auto index = (unsigned int)rand();
  QRYIndex qry(index + 1, index + 2);
  ASSERT_THROW(qry.setLastIndex(index), Exception);
}

TEST(Query, QRYIndex_Set_Single_Index) {
  auto index = (unsigned int)rand();
  QRYIndex qry;
  qry.setIndex(index);
  ASSERT_EQ(index, qry.firstIndex());
  ASSERT_EQ(index + 1, qry.lastIndex());
}

TEST(Query, QRYIndex_Set_Single_Index_Overflow) {
  auto index = UINT32_MAX;
  QRYIndex qry;
  ASSERT_THROW(qry.setIndex(index), Exception);
}

TEST(Query, QRYIndex_Set_Range) {
  auto index = (unsigned int)rand();
  auto len = rand() % 100;
  QRYIndex qry;
  qry.setRange(index, len);
  ASSERT_EQ(index, qry.firstIndex());
  ASSERT_EQ(index + len, qry.lastIndex());
  ASSERT_EQ(len, qry.length());
}

TEST(Query, QRYIndex_Set_Range_Backwards) {
  int index = (rand() % 10000) + 1000;
  int len = rand() % 1000;
  QRYIndex qry;
  qry.setRange(index, -len);
  ASSERT_EQ(index - len + 1, qry.firstIndex());
  ASSERT_EQ(index + 1, qry.lastIndex());
  ASSERT_EQ(len, qry.length());
}

TEST(Query, QRYIndex_Set_Range_With_Max_Possible_Value) {
  unsigned int index = UINT32_MAX - 1;
  int len = 100;
  QRYIndex qry;
  qry.setRange(index, -len);
  ASSERT_EQ(UINT32_MAX - len, qry.firstIndex());
  ASSERT_EQ(UINT32_MAX, qry.lastIndex());
  ASSERT_EQ(len, qry.length());
}

TEST(Query, QRYIndex_Set_Range_With_Overflowing_Value) {
  unsigned int index = UINT32_MAX;
  QRYIndex qry;
  ASSERT_THROW(qry.setRange(index, 1), Exception);
}

TEST(Query, QRYIndex_Set_Range_With_Underflowing_Value) {
  unsigned int index = 0;
  QRYIndex qry;
  ASSERT_THROW(qry.setRange(index, -2), Exception);
}

TEST(Query, QRYIndex_By_Range) {
  unsigned int index1 = (unsigned int)(rand() % 1000) + 100;
  unsigned int index2 = index1 + (rand() % 100);
  auto qry = QRYIndex::byRange(index1, index2);
  QRYIndex expQry(index1, index2);
  ASSERT_EQ(expQry.firstIndex(), qry.firstIndex());
  ASSERT_EQ(expQry.lastIndex(), qry.lastIndex());
}

TEST(Query, QRYIndex_By_Range_With_Length) {
  unsigned int index = (unsigned int)(rand() % 1000) + 100;
  int len = rand() % 100;
  auto qry = QRYIndex::byRange(index, len);
  QRYIndex expQry(index, len);
  ASSERT_EQ(expQry.firstIndex(), qry.firstIndex());
  ASSERT_EQ(expQry.lastIndex(), qry.lastIndex());
}

TEST(Query, QRYIndex_By_Index) {
  unsigned int index = (unsigned int)(rand() % 1000) + 100;
  auto qry = QRYIndex::byIndex(index);
  ASSERT_EQ(index, qry.firstIndex());
  ASSERT_EQ(index + 1, qry.lastIndex());
}

struct QueryTest_Parameter {
  int _x;
  int _y;
};

TEST(Query, QRYParameterValue_Default_Constructor) {
  QRYParameterValue<QueryTest_Parameter> qry;
  ASSERT_EQ(0, qry.value()._x);
  ASSERT_EQ(0, qry.value()._y);
}

TEST(Query, QRYParameterValue_Constructor) {
  QueryTest_Parameter param = {rand(), rand()};
  QRYParameterValue<QueryTest_Parameter> qry(param);
  ASSERT_EQ(param._x, qry.value()._x);
  ASSERT_EQ(param._y, qry.value()._y);
}