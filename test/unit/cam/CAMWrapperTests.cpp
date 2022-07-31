#include <CAMWrapper.hpp>
#include <Exception.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;
using namespace level::cam;

TEST(CAMWrapper, SuccessfulConstruction) {
  CAMWrapper cam(1);
  ASSERT_EQ(1, cam->header.stationID);
}

TEST(CAMWrapper, ArrowOperator) {
  CAMWrapper cam(1);
  cam->header.stationID = 2;
  ASSERT_EQ(2, cam->header.stationID);
}

TEST(CAMWrapper, InitializeLowFrequencyContainer) {
  CAMWrapper cam(1);
  auto type = LowFrequencyContainer_PR_basicVehicleContainerLowFrequency;
  cam.setLFC(type);

  ASSERT_NE(nullptr, cam->cam.camParameters.lowFrequencyContainer);
  ASSERT_EQ(type, cam->cam.camParameters.lowFrequencyContainer->present);
}

TEST(CAMWrapper, OverrideLowFrequencyContainer) {
  CAMWrapper cam(1);
  cam.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  cam.setLFC(LowFrequencyContainer_PR_NOTHING);

  ASSERT_EQ(nullptr, cam->cam.camParameters.lowFrequencyContainer);
}

TEST(CAMWrapper, FailsOnCopyingInvalidMessage) {
  CAMWrapper cam1(1);
  cam1->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_NOTHING;
  ASSERT_THROW(CAMWrapper cam2(cam1), Exception);
}