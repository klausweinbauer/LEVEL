#include <DataExtension.hpp>
#include <Exception.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace level::DataExtensionTests {

enum ContainerType { Type1, Type2 };

struct ContainerA {
  int a;
};

struct ContainerB {
  int b;
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
  ContainerA cr;
  ContainerB *cp;
  ContainerModel cru;
  ContainerModel *cpu;

  Model() : pp(nullptr), cp(nullptr), cpu(nullptr) {}
  ~Model() {
    if (pp) {
      free(pp);
    }
    if (cp) {
      free(cp);
    }
    if (cpu) {
      free(cpu);
    }
  }
};

DEF_RP(int, Model, rp, RP)

#define asn_DEF_int
#define ASN_STRUCT_FREE(def, ptr) free(ptr)
DEF_PP(int, Model, pp, PP)

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