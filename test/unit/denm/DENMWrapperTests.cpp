#include <DENMExtensions.hpp>
#include <Exception.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

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

TEST(DENMWrapper, CopyConstruction) {}

TEST(DENMWrapper, InitializeLowFrequencyContainer) {}

TEST(DENMWrapper, OverrideLowFrequencyContainer) {}

TEST(DENMWrapper, AssignmentOperator) {}

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

// TODO Find a configuration that is not encodable
// TODO Try cause code in situation container
/*TEST(DENMWrapper, FailsOnCopyingInvalidMessage) {
  DENMWrapper denm(1, rand());
  ASSERT_THROW(DENMWrapper denmNew(denm), Exception);
}*/