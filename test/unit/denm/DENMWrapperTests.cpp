#include <DENMEncoder.hpp>
#include <DENMExtensions.hpp>
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

TEST(DENMWrapper, ArrowOperator) {
  DENMWrapper denm(1, rand());
  denm->header.stationID = 2;
  ASSERT_EQ(2, denm->header.stationID);
}

TEST(DENMWrapper, StarOperator) {
  DENMWrapper denm(1, rand());
  denm->header.stationID = 2;
  ASSERT_EQ(2, (*denm).header.stationID);
}

TEST(DENMWrapper, CopyConstruction) {
  DENMWrapper denm;
  CauseCode cc{0, 0};
  cc.causeCode = rand();
  setEventType(getSituation(denm), cc);
  DENMWrapper denmCopy(denm);
  CauseCode cc2{0, 0};
  cc2.causeCode = cc.causeCode + 1;
  setEventType(getSituation(denmCopy), cc2);
  ASSERT_EQ(cc.causeCode, getEventType(getSituation(denm)).causeCode);
  ASSERT_EQ(cc2.causeCode, getEventType(getSituation(denmCopy)).causeCode);
}

TEST(DENMWrapper, AssignmentOperator) {
  DENMWrapper denm1;
  DENMWrapper denm2;
  InformationQuality_t q1 = rand();
  InformationQuality_t q2 = q1 + 1;
  setInfromationQuality(getSituation(denm1), q1);

  denm2 = denm1;
  ASSERT_EQ(q1, getInfromationQuality(getSituation(denm1)));
  ASSERT_EQ(q1, getInfromationQuality(getSituation(denm2)));

  setInfromationQuality(getSituation(denm2), q2);
  ASSERT_EQ(q1, getInfromationQuality(getSituation(denm1)));
  ASSERT_EQ(q2, getInfromationQuality(getSituation(denm2)));
}

TEST(DENMWrapper, GetSituationContainer) {
  DENMWrapper denm;
  ASSERT_NE(nullptr, getSituation(denm));
}

TEST(DENMWrapper, GetAndSetExtensionMethods) {
  DENMWrapper denm;
  CauseCode cc;
  cc.causeCode = CauseCodeType_aquaplannning;
  setEventType(getSituation(denm), cc);
  ASSERT_EQ(CauseCodeType_aquaplannning,
            getEventType(getSituation(denm)).causeCode);
  ASSERT_EQ(denm->denm.situation->eventType.causeCode,
            getEventType(getSituation(denm)).causeCode);
}

TEST(DENMWrapper, ClearContainer) {
  DENMWrapper denm;
  CauseCode cc;
  cc.causeCode = CauseCodeType_aquaplannning;
  setEventType(getSituation(denm), cc);
  clearSituation(*denm);
  ASSERT_NE(CauseCodeType::CauseCodeType_aquaplannning,
            getEventType(getSituation(denm)).causeCode);
}

TEST(DENMWrapper, GetDENMTypename) {
  ASSERT_EQ(std::string(DENM_TYPE_NAME), std::string(TypeName<DENM>::get()));
}
TEST(DENMWrapper, GetDENMWrapperTypename) {
  ASSERT_EQ(std::string(DENM_TYPE_NAME),
            std::string(TypeName<DENMWrapper>::get()));
}