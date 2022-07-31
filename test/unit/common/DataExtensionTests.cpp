#include <DataExtension.hpp>
#include <Exception.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace level::DataExtensionTests {

enum ContainerType { TypeA, TypeB };

struct ContainerA {
  int a;
};

struct ContainerB {
  float b;
};

struct ContainerModel {
  ContainerType present;
  union {
    ContainerA containerA;
    ContainerB containerB;
  } choice;
};

struct Model {
  int rp;
  int *pp;
  ContainerModel rpu;
  ContainerModel *ppu;

  Model() : pp(nullptr), ppu(nullptr) {}
  ~Model() {
    if (pp) {
      free(pp);
    }
    if (ppu) {
      free(ppu);
    }
  }
};

// Define reference parameter
DEF_RP(int, Model, rp, RP)

// Define pointer parameter
#define asn_DEF_int
#define ASN_STRUCT_FREE(def, ptr) free(ptr)
DEF_PP(int, Model, pp, PP)

// Define union reference parameter
DEF_RPU(ContainerA, Model, rpu, TypeA, TypeA, containerA)
DEF_RPU(ContainerB, Model, rpu, TypeB, TypeB, containerB)

} // namespace level::DataExtensionTests

using namespace level;
using namespace level::DataExtensionTests;

// Reference property
TEST(DataExtension, GetReferencePropertyByParentPointer) {
  Model m;
  m.rp = rand();
  ASSERT_NE(nullptr, getRP(&m));
  ASSERT_EQ(m.rp, *getRP(&m));
}

TEST(DataExtension, GetReferencePropertyByParentReference) {
  Model m;
  m.rp = rand();
  ASSERT_NE(nullptr, getRP(m));
  ASSERT_EQ(m.rp, *getRP(m));
}

TEST(DataExtension, SetReferencePropertyByParentPointer) {
  Model m;
  int value = rand();
  ASSERT_NO_THROW(setRP(&m, value));
  ASSERT_EQ(value, *getRP(&m));
}

TEST(DataExtension, SetReferencePropertyByParentReference) {
  Model m;
  int value = rand();
  ASSERT_NO_THROW(setRP(m, value));
  ASSERT_EQ(value, *getRP(m));
}

// Pointer property
TEST(DataExtension, GetPointerPropertyByParentPointer) {
  Model m;
  ASSERT_EQ(nullptr, getPP(&m));
}

TEST(DataExtension, GetPointerPropertyByParentReference) {
  Model m;
  ASSERT_EQ(nullptr, getPP(m));
}

TEST(DataExtension, SetPointerPropertyByParentPointer) {
  Model m;
  int value = rand();
  ASSERT_NO_THROW(setPP(&m, value));
  ASSERT_EQ(value, *getPP(&m));
}

TEST(DataExtension, SetPointerPropertyByParentReference) {
  Model m;
  int value = rand();
  ASSERT_NO_THROW(setPP(m, value));
  ASSERT_EQ(value, *getPP(m));
}

TEST(DataExtension, DoubleSetPointerPropertyByParentPointer) {
  Model m;
  int value = rand();
  setPP(&m, value);
  ASSERT_NO_THROW(setPP(&m, value + 1));
}

TEST(DataExtension, DoubleSetPointerPropertyByParentReference) {
  Model m;
  int value = rand();
  setPP(m, value);
  ASSERT_NO_THROW(setPP(m, value + 1));
}

TEST(DataExtension, ClearPointerPropertyByParentPointer) {
  Model m;
  setPP(&m, 0);
  ASSERT_NO_THROW(clearPP(&m));
  ASSERT_EQ(nullptr, getPP(&m));
}

TEST(DataExtension, ClearPointerPropertyByParentReference) {
  Model m;
  setPP(m, 0);
  ASSERT_NO_THROW(clearPP(m));
  ASSERT_EQ(nullptr, getPP(m));
}

// Union reference property
TEST(DataExtension, GetUnionReferencePropertyByParentPointer) {
  Model m;
  int value = rand();
  m.rpu.present = TypeA;
  m.rpu.choice.containerA.a = value;
  ASSERT_NE(nullptr, getTypeA(&m));
  ASSERT_EQ(value, getTypeA(&m)->a);
}

TEST(DataExtension, GetUnionReferencePropertyByParentReference) {
  Model m;
  float value = (float)(rand() / 3.1415926535);
  m.rpu.present = TypeB;
  m.rpu.choice.containerB.b = value;
  ASSERT_NE(nullptr, getTypeB(m));
  ASSERT_EQ(value, getTypeB(m)->b);
}

TEST(DataExtension, SetUnionReferencePropertyByParentPointer) {
  Model m;
  ASSERT_NO_THROW(setTypeA(&m));
}

TEST(DataExtension, SetUnionReferencePropertyByParentReference) {
  Model m;
  ASSERT_NO_THROW(setTypeB(m));
}

TEST(DataExtension, ResetUnionReferencePropertyByParentPointer) {
  Model m;
  setTypeA(&m);
  getTypeA(&m)->a = rand();
  setTypeB(&m);
  ASSERT_EQ(0.0, getTypeB(&m)->b);
}

TEST(DataExtension, ResetUnionReferencePropertyByParentReference) {
  Model m;
  setTypeB(m);
  getTypeB(m)->b = (rand() / 3.1415926535);
  setTypeA(m);
  ASSERT_EQ(0, getTypeA(m)->a);
}

TEST(DataExtension, ThrowOnGetWrongUnionReferencePropertyByParentPointer) {
  Model m;
  setTypeA(&m);
  ASSERT_THROW(getTypeB(&m), Exception);
}

TEST(DataExtension, ThrowOnGetWrongUnionReferencePropertyByParentReference) {
  Model m;
  setTypeB(m);
  ASSERT_THROW(getTypeA(m), Exception);
}

TEST(DataExtension, OverrideUnionReferencePropertyByParentPointer) {
  Model m;
  setTypeA(&m);
  setTypeB(&m);
  ASSERT_NO_THROW(getTypeB(&m));
}

TEST(DataExtension, OverrideUnionReferencePropertyByParentReference) {
  Model m;
  setTypeB(m);
  setTypeA(m);
  ASSERT_NO_THROW(getTypeA(m));
}