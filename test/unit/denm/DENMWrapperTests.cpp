#include <DENMEncoder.hpp>
#include <DENMWrapper.hpp>
#include <Exception.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using namespace level;
using namespace level::denm;

TEST(DENMWrapper, SuccessfulConstruction) {
  int id = rand();
  int seqNr = rand();
  DENMWrapper denm(id, seqNr);
  ASSERT_EQ(id, denm->header.stationID);
  ASSERT_EQ(id, denm->denm.management.actionID.originatingStationID);
  ASSERT_EQ(seqNr, denm->denm.management.actionID.sequenceNumber);
}

TEST(DENMWrapper, InitializeWithPointer) {
  int id = rand();
  DENM *denmPtr = (DENM *)calloc(1, sizeof(DENM));
  denmPtr->header.messageID = id;
  DENMWrapper denm(denmPtr);
  ASSERT_EQ(id, denm->header.messageID);
}

TEST(DENMWrapper, ArrowOperator) {
  DENMWrapper denm(1, rand());
  denm->header.stationID = 2;
  ASSERT_EQ(2, denm->header.stationID);
}

TEST(DENMWrapper, PointerOperator) {
  DENMWrapper denm(1, rand());
  denm->header.stationID = 2;
  ASSERT_EQ(2, (*denm).header.stationID);
}

TEST(DENMWrapper, GetDENM) {
  int id = rand();
  DENMWrapper denm(id, 0);
  ASSERT_EQ(id, denm.get()->header.stationID);
}

TEST(DENMWrapper, CopyConstructor) {
  DENMWrapper denm1;
  denm1.setSC();
  DENMWrapper denm2(denm1);
  ASSERT_NE(nullptr, denm1.getSC());
  ASSERT_NE(nullptr, denm2.getSC());
  ASSERT_NE(denm2.getSC(), denm1.getSC());
}

TEST(DENMWrapper, AssignmentOperator) {
  DENMWrapper denm1;
  denm1.setSC();
  DENMWrapper denm2;
  ASSERT_EQ(nullptr, denm2.getSC());
  denm2 = denm1;
  ASSERT_NE(nullptr, denm1.getSC());
  ASSERT_NE(nullptr, denm2.getSC());
  ASSERT_NE(denm2.getSC(), denm1.getSC());
}

TEST(DENMWrapper, GetMC) {
  DENMWrapper denm;
  ASSERT_EQ(&denm->denm.management, denm.getMC());
}

TEST(DENMWrapper, ClearMC) {
  DENMWrapper denm;
  denm.getMC()->actionID.originatingStationID = rand();
  denm.clearMC();
  ASSERT_EQ(0, denm.getMC()->actionID.originatingStationID);
}

TEST(DENMWrapper, GetUninitializedSC) {
  DENMWrapper denm;
  ASSERT_EQ(nullptr, denm.getSC());
}

TEST(DENMWrapper, SetSC) {
  DENMWrapper denm;
  auto sc = denm.setSC();
  ASSERT_NE(nullptr, sc);
  ASSERT_EQ(denm->denm.situation, sc);
}

TEST(DENMWrapper, ThrowOnResetSC) {
  DENMWrapper denm;
  denm.setSC();
  ASSERT_THROW(denm.setSC(), Exception);
}

TEST(DENMWrapper, GetSC) {
  DENMWrapper denm;
  denm.setSC();
  ASSERT_EQ(denm->denm.situation, denm.getSC());
}

TEST(DENMWrapper, ClearSC) {
  DENMWrapper denm;
  denm.setSC();
  denm.clearSC();
  ASSERT_EQ(nullptr, denm.getSC());
}

TEST(DENMWrapper, ClearUninitializedSC) {
  DENMWrapper denm;
  ASSERT_NO_THROW(denm.clearSC());
}

TEST(DENMWrapper, GetUninitializedLC) {
  DENMWrapper denm;
  ASSERT_EQ(nullptr, denm.getLC());
}

TEST(DENMWrapper, SetLC) {
  DENMWrapper denm;
  auto lc = denm.setLC();
  ASSERT_NE(nullptr, lc);
  ASSERT_EQ(denm->denm.location, lc);
}

TEST(DENMWrapper, ThrowOnResetLC) {
  DENMWrapper denm;
  denm.setLC();
  ASSERT_THROW(denm.setLC(), Exception);
}

TEST(DENMWrapper, GetLC) {
  DENMWrapper denm;
  denm.setLC();
  ASSERT_EQ(denm->denm.location, denm.getLC());
}

TEST(DENMWrapper, ClearLC) {
  DENMWrapper denm;
  denm.setLC();
  denm.clearLC();
  ASSERT_EQ(nullptr, denm.getLC());
}

TEST(DENMWrapper, ClearUninitializedLC) {
  DENMWrapper denm;
  ASSERT_NO_THROW(denm.clearLC());
}

TEST(DENMWrapper, GetUninitializedAC) {
  DENMWrapper denm;
  ASSERT_EQ(nullptr, denm.getAC());
}

TEST(DENMWrapper, SetAC) {
  DENMWrapper denm;
  auto ac = denm.setAC();
  ASSERT_NE(nullptr, ac);
  ASSERT_EQ(denm->denm.alacarte, ac);
}

TEST(DENMWrapper, ThrowOnResetAC) {
  DENMWrapper denm;
  denm.setAC();
  ASSERT_THROW(denm.setAC(), Exception);
}

TEST(DENMWrapper, GetAC) {
  DENMWrapper denm;
  denm.setAC();
  ASSERT_EQ(denm->denm.alacarte, denm.getAC());
}

TEST(DENMWrapper, ClearAC) {
  DENMWrapper denm;
  denm.setAC();
  denm.clearAC();
  ASSERT_EQ(nullptr, denm.getAC());
}

TEST(DENMWrapper, ClearUninitializedAC) {
  DENMWrapper denm;
  ASSERT_NO_THROW(denm.clearAC());
}