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