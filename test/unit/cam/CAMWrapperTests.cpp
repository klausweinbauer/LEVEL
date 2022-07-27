#include <CAMWrapper.hpp>
#include <Exception.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;
using namespace level::cam;

TEST(CAMWrapper, TestCtor) {
  CAMWrapper cam(1);
  ASSERT_EQ(1, cam->header.stationID);
}

TEST(CAMWrapper, TestArrowOperator) {
  CAMWrapper cam(1);
  cam->header.stationID = 2;
  ASSERT_EQ(2, cam->header.stationID);
}

TEST(CAMWrapper, TestCopyCtor) {
  CAMWrapper cam1(1);
  cam1.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  auto lfc1 = &cam1->cam.camParameters.lowFrequencyContainer->choice
                   .basicVehicleContainerLowFrequency;
  lfc1->vehicleRole = VehicleRole_publicTransport;

  CAMWrapper cam2(cam1);
  auto lfc2 = &cam2->cam.camParameters.lowFrequencyContainer->choice
                   .basicVehicleContainerLowFrequency;
  ASSERT_EQ(VehicleRole_publicTransport, lfc2->vehicleRole);

  lfc2->vehicleRole = VehicleRole_specialTransport;
  ASSERT_EQ(VehicleRole_publicTransport, lfc1->vehicleRole);
  ASSERT_EQ(VehicleRole_specialTransport, lfc2->vehicleRole);
}

TEST(CAMWrapper, TestInitLFC) {
  CAMWrapper cam(1);
  auto type = LowFrequencyContainer_PR_basicVehicleContainerLowFrequency;
  cam.setLFC(type);

  ASSERT_NE(nullptr, cam->cam.camParameters.lowFrequencyContainer);
  ASSERT_EQ(type, cam->cam.camParameters.lowFrequencyContainer->present);
}

TEST(CAMWrapper, TestOverrideLFC) {
  CAMWrapper cam(1);
  cam.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  cam.setLFC(LowFrequencyContainer_PR_NOTHING);

  ASSERT_EQ(nullptr, cam->cam.camParameters.lowFrequencyContainer);
}

TEST(CAMWrapper, TestAssignmentOperator) {
  CAMWrapper cam1(1);
  cam1.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  auto lfc1 = &cam1->cam.camParameters.lowFrequencyContainer->choice
                   .basicVehicleContainerLowFrequency;
  lfc1->vehicleRole = VehicleRole_publicTransport;

  CAMWrapper cam2(2);
  ASSERT_EQ(2, cam2->header.stationID);
  cam2 = cam1;
  auto lfc2 = &cam2->cam.camParameters.lowFrequencyContainer->choice
                   .basicVehicleContainerLowFrequency;
  ASSERT_EQ(1, cam2->header.stationID);
  ASSERT_EQ(VehicleRole_publicTransport, lfc2->vehicleRole);

  lfc2->vehicleRole = VehicleRole_specialTransport;
  ASSERT_EQ(VehicleRole_publicTransport, lfc1->vehicleRole);
  ASSERT_EQ(VehicleRole_specialTransport, lfc2->vehicleRole);
}

TEST(CAMWrapper, TestFailedCopy) {
  CAMWrapper cam1(1);
  cam1->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_NOTHING;
  bool threwException = false;

  try {
    CAMWrapper cam2(cam1);
  } catch (const Exception &) {
    threwException = true;
  }

  ASSERT_TRUE(threwException);
}