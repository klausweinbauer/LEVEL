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

// Define reference property
DEF_RP(int, Model, rp, RP)

// Define pointer property
#define asn_DEF_int
#define ASN_STRUCT_FREE(def, ptr) free(ptr)
DEF_PP(int, Model, pp, PP)

// Define union reference property
DEF_RPU(ContainerModel, ContainerA, Model, rpu, RPUTypeA, TypeA, containerA)
DEF_RPU(ContainerModel, ContainerB, Model, rpu, RPUTypeB, TypeB, containerB)

// Define union pointer property
#define asn_DEF_ContainerModel
#define ASN_STRUCT_FREE(def, ptr) free(ptr)
DEF_PPU(ContainerModel, ContainerA, Model, ppu, PPUTypeA, TypeA, containerA)
DEF_PPU(ContainerModel, ContainerB, Model, ppu, PPUTypeB, TypeB, containerB)
DEF_CLEAR_PPU(ContainerModel, Model, ppu, PPU);

} // namespace level::DataExtensionTests

using namespace level;
using namespace level::DataExtensionTests;

// Reference property tests
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

// Pointer property tests
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

// Union reference property tests
TEST(DataExtension, GetUnionReferencePropertyByParentPointer) {
  Model m;
  int value = rand();
  m.rpu.present = TypeA;
  m.rpu.choice.containerA.a = value;
  ASSERT_NE(nullptr, getRPUTypeA(&m));
  ASSERT_EQ(value, getRPUTypeA(&m)->a);
}

TEST(DataExtension, GetUnionReferencePropertyByParentReference) {
  Model m;
  float value = (float)(rand() / 3.1415926535);
  m.rpu.present = TypeB;
  m.rpu.choice.containerB.b = value;
  ASSERT_NE(nullptr, getRPUTypeB(m));
  ASSERT_EQ(value, getRPUTypeB(m)->b);
}

TEST(DataExtension, SetUnionReferencePropertyByParentPointer) {
  Model m;
  ASSERT_NO_THROW(setRPUTypeA(&m));
  ASSERT_EQ(TypeA, m.rpu.present);
}

TEST(DataExtension, SetUnionReferencePropertyByParentReference) {
  Model m;
  ASSERT_NO_THROW(setRPUTypeB(m));
  ASSERT_EQ(TypeB, m.rpu.present);
}

TEST(DataExtension, ResetUnionReferencePropertyByParentPointer) {
  Model m;
  setRPUTypeA(&m);
  getRPUTypeA(&m)->a = rand();
  setRPUTypeB(&m);
  ASSERT_EQ(0.0, getRPUTypeB(&m)->b);
}

TEST(DataExtension, ResetUnionReferencePropertyByParentReference) {
  Model m;
  setRPUTypeB(m);
  getRPUTypeB(m)->b = (rand() / 3.1415926535);
  setRPUTypeA(m);
  ASSERT_EQ(0, getRPUTypeA(m)->a);
}

TEST(DataExtension, ThrowOnGetWrongUnionReferencePropertyByParentPointer) {
  Model m;
  setRPUTypeA(&m);
  ASSERT_THROW(getRPUTypeB(&m), Exception);
}

TEST(DataExtension, ThrowOnGetWrongUnionReferencePropertyByParentReference) {
  Model m;
  setRPUTypeB(m);
  ASSERT_THROW(getRPUTypeA(m), Exception);
}

TEST(DataExtension, OverrideUnionReferencePropertyByParentPointer) {
  Model m;
  setRPUTypeA(&m);
  setRPUTypeB(&m);
  ASSERT_NO_THROW(getRPUTypeB(&m));
}

TEST(DataExtension, OverrideUnionReferencePropertyByParentReference) {
  Model m;
  setRPUTypeB(m);
  setRPUTypeA(m);
  ASSERT_NO_THROW(getRPUTypeA(m));
}

// Union pointer property tests
TEST(DataExtension, GetUnionPointerPropertyByParentPointer) {
  Model m;
  ASSERT_EQ(nullptr, getPPUTypeA(&m));
}

TEST(DataExtension, GetUnionPointerPropertyByParentReference) {
  Model m;
  ASSERT_EQ(nullptr, getPPUTypeB(m));
}

TEST(DataExtension, SetUnionPointerPropertyByParentPointer) {
  Model m;
  ASSERT_NO_THROW(setPPUTypeA(&m));
  ASSERT_NE(nullptr, getPPUTypeA(&m));
}

TEST(DataExtension, SetUnionPointerPropertyByParentReference) {
  Model m;
  ASSERT_NO_THROW(setPPUTypeB(m));
  ASSERT_NE(nullptr, getPPUTypeB(m));
}

TEST(DataExtension, ResetUnionPointerPropertyByParentPointer) {
  Model m;
  setPPUTypeA(&m);
  ASSERT_THROW(setPPUTypeB(&m), Exception);
}

TEST(DataExtension, ResetUnionPointerPropertyByParentReference) {
  Model m;
  setPPUTypeB(m);
  ASSERT_THROW(setPPUTypeA(m), Exception);
}

TEST(DataExtension, ThrowOnGetWrongUnionPointerPropertyByParentPointer) {
  Model m;
  setPPUTypeA(&m);
  ASSERT_THROW(getPPUTypeB(&m), Exception);
}

TEST(DataExtension, ThrowOnGetWrongUnionPointerPropertyByParentReference) {
  Model m;
  setPPUTypeB(m);
  ASSERT_THROW(getPPUTypeA(m), Exception);
}

TEST(DataExtension, ClearUnionPointerPropertyByParentPointer) {
  Model m;
  setPPUTypeA(&m);
  ASSERT_NO_THROW(clearPPU(&m));
  ASSERT_EQ(nullptr, getPPUTypeA(&m));
}

TEST(DataExtension, ClearUnionPointerPropertyByParentReference) {
  Model m;
  setPPUTypeB(m);
  ASSERT_NO_THROW(clearPPU(m));
  ASSERT_EQ(nullptr, getPPUTypeA(m));
}