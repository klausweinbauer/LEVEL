#include <DENMWrapper.hpp>
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

TEST(DENMWrapper, CopyConstruction) {}

TEST(DENMWrapper, InitializeLowFrequencyContainer) {}

TEST(DENMWrapper, OverrideLowFrequencyContainer) {}

TEST(DENMWrapper, AssignmentOperator) {}

TEST(DENMWrapper, GetSituationContainer) {
  DENMWrapper denm;
  ASSERT_NE(nullptr, denm.getSituationContainer());
}

TEST(DENMWrapper, DontOverrideSituationContainerOnGet) {
  DENMWrapper denm;
  auto container = denm.getSituationContainer();
  container->eventType.causeCode = CauseCodeType::CauseCodeType_aquaplannning;
  ASSERT_EQ(container->eventType.causeCode,
            denm.getSituationContainer()->eventType.causeCode);
}

TEST(DENMWrapper, ClearSituationContainer) {
  DENMWrapper denm;
  auto container = denm.getSituationContainer();
  container->eventType.causeCode = CauseCodeType::CauseCodeType_aquaplannning;
  denm.clearSituationContainer();
  ASSERT_NE(CauseCodeType::CauseCodeType_aquaplannning,
            denm.getSituationContainer()->eventType.causeCode);
}

TEST(DENMWrapper, GetLocationContainer) {
  DENMWrapper denm;
  ASSERT_NE(nullptr, denm.getLocationContainer());
}

TEST(DENMWrapper, DontOverrideLocationContainerOnGet) {
  DENMWrapper denm;
  auto container = denm.getLocationContainer();
  container->roadType = (RoadType_t *)calloc(1, sizeof(RoadType_t));
  *container->roadType = RoadType_urban_NoStructuralSeparationToOppositeLanes;
  ASSERT_EQ(*container->roadType, *denm.getLocationContainer()->roadType);
}

TEST(DENMWrapper, ClearLocationContainer) {
  DENMWrapper denm;
  auto container = denm.getLocationContainer();
  container->roadType = (RoadType_t *)calloc(1, sizeof(RoadType_t));
  denm.clearLocationContainer();
  ASSERT_EQ(nullptr, denm.getLocationContainer()->roadType);
}

TEST(DENMWrapper, GetAlacarteContainer) {
  DENMWrapper denm;
  ASSERT_NE(nullptr, denm.getAlacarteContainer());
}

TEST(DENMWrapper, DontOverrideAlacarteContainerOnGet) {
  DENMWrapper denm;
  auto container = denm.getAlacarteContainer();
  container->lanePosition = (LanePosition_t *)calloc(1, sizeof(LanePosition_t));
  *container->lanePosition = LanePosition_secondLaneFromOutside;
  ASSERT_EQ(*container->lanePosition,
            *denm.getAlacarteContainer()->lanePosition);
}

TEST(DENMWrapper, ClearAlacarteContainer) {
  DENMWrapper denm;
  auto container = denm.getAlacarteContainer();
  container->lanePosition = (LanePosition_t *)calloc(1, sizeof(LanePosition_t));
  denm.clearAlacarteContainer();
  ASSERT_EQ(nullptr, denm.getAlacarteContainer()->lanePosition);
}

// TODO Find a configuration that is not encodable
// TODO Try cause code in situation container
/*TEST(DENMWrapper, FailsOnCopyingInvalidMessage) {
  DENMWrapper denm(1, rand());
  ASSERT_THROW(DENMWrapper denmNew(denm), Exception);
}*/