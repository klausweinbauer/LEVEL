#include <CABasicService.hpp>
#include <Mocks.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

namespace level::CABasicServiceTests {

float randFloat() { return rand() * 3.1415962535; }

std::shared_ptr<NiceMock<MValueConverter>> getConverter() {
  return std::make_shared<NiceMock<MValueConverter>>();
}

} // namespace level::CABasicServiceTests

using namespace level::cam;
using namespace level::CABasicServiceTests;

TEST(CABasicService, SetHeading) {
  auto converter = getConverter();
  float siValue = randFloat();
  int itsValue = rand();
  EXPECT_CALL(*converter, siToITSHeading(siValue)).WillOnce(Return(itsValue));
  CABasicService service(converter);
  service.setHeading(siValue);
  EXPECT_EQ(itsValue,
            service.cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.heading.headingValue);
}

TEST(CABasicService, IgnoreSetHeadingForInvalidHFC) {
  auto converter = getConverter();
  EXPECT_CALL(*converter, siToITSHeading(_)).Times(0);
  CABasicService service(converter);
  service.cam().setHFC(HighFrequencyContainer_PR_NOTHING);
  service.setHeading(randFloat());
  ASSERT_EQ(0, service.cam()
                   ->cam.camParameters.highFrequencyContainer.choice
                   .basicVehicleContainerHighFrequency.heading.headingValue);
}

TEST(CABasicService, SetSpeed) {
  auto converter = getConverter();
  float siValue = randFloat();
  int itsValue = rand();
  EXPECT_CALL(*converter, siToITSSpeed(siValue)).WillOnce(Return(itsValue));
  CABasicService service(converter);
  service.setSpeed(siValue);
  EXPECT_EQ(itsValue, service.cam()
                          ->cam.camParameters.highFrequencyContainer.choice
                          .basicVehicleContainerHighFrequency.speed.speedValue);
}

TEST(CABasicService, IgnoreSetSpeedForInvalidHFC) {
  auto converter = getConverter();
  EXPECT_CALL(*converter, siToITSSpeed(_)).Times(0);
  CABasicService service(converter);
  service.cam().setHFC(HighFrequencyContainer_PR_NOTHING);
  service.setSpeed(randFloat());
  ASSERT_EQ(0, service.cam()
                   ->cam.camParameters.highFrequencyContainer.choice
                   .basicVehicleContainerHighFrequency.speed.speedValue);
}

TEST(CABasicService, SetDriveDirection) {
  auto converter = getConverter();
  DriveDirectionType direction = DriveDirectionType::DirveDirection_Backward;
  CABasicService service(converter);
  service.setDriveDirection(direction);
  EXPECT_EQ(direction, service.cam()
                           ->cam.camParameters.highFrequencyContainer.choice
                           .basicVehicleContainerHighFrequency.driveDirection);
}

TEST(CABasicService, IgnoreSetDirectionForInvalidHFC) {
  auto converter = getConverter();
  CABasicService service(converter);
  service.cam().setHFC(HighFrequencyContainer_PR_NOTHING);
  service.setDriveDirection(DriveDirectionType::DirveDirection_Backward);
  ASSERT_EQ(0, service.cam()
                   ->cam.camParameters.highFrequencyContainer.choice
                   .basicVehicleContainerHighFrequency.driveDirection);
}

TEST(CABasicService, SetAcceleration) {
  auto converter = getConverter();
  float siValue = randFloat();
  int itsValue = rand();
  EXPECT_CALL(*converter, siToITSLongitudinalAcceleration(siValue))
      .WillOnce(Return(itsValue));
  CABasicService service(converter);
  service.setAcceleration(siValue);
  EXPECT_EQ(itsValue,
            service.cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.longitudinalAcceleration
                .longitudinalAccelerationValue);
}

TEST(CABasicService, IgnoreSetAccelerationForInvalidHFC) {
  auto converter = getConverter();
  EXPECT_CALL(*converter, siToITSLongitudinalAcceleration(_)).Times(0);
  CABasicService service(converter);
  service.cam().setHFC(HighFrequencyContainer_PR_NOTHING);
  service.setAcceleration(randFloat());
  ASSERT_EQ(0, service.cam()
                   ->cam.camParameters.highFrequencyContainer.choice
                   .basicVehicleContainerHighFrequency.longitudinalAcceleration
                   .longitudinalAccelerationValue);
}

TEST(CABasicService, SetCurvature) {
  auto converter = getConverter();
  float siValue = randFloat();
  int itsValue = rand();
  EXPECT_CALL(*converter, siToITSCurvature(siValue)).WillOnce(Return(itsValue));
  CABasicService service(converter);
  service.setCurvature(siValue);
  EXPECT_EQ(itsValue,
            service.cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.curvature.curvatureValue);
}

TEST(CABasicService, IgnoreSetCurvatureForInvalidHFC) {
  auto converter = getConverter();
  EXPECT_CALL(*converter, siToITSCurvature(_)).Times(0);
  CABasicService service(converter);
  service.cam().setHFC(HighFrequencyContainer_PR_NOTHING);
  service.setCurvature(randFloat());
  ASSERT_EQ(0,
            service.cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.curvature.curvatureValue);
}

TEST(CABasicService, SetYawRate) {
  auto converter = getConverter();
  float siValue = randFloat();
  int itsValue = rand();
  EXPECT_CALL(*converter, siToITSYawRate(siValue)).WillOnce(Return(itsValue));
  CABasicService service(converter);
  service.setYawRate(siValue);
  EXPECT_EQ(itsValue,
            service.cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.yawRate.yawRateValue);
}

TEST(CABasicService, IgnoreSetYawRateForInvalidHFC) {
  auto converter = getConverter();
  EXPECT_CALL(*converter, siToITSYawRate(_)).Times(0);
  CABasicService service(converter);
  service.cam().setHFC(HighFrequencyContainer_PR_NOTHING);
  service.setYawRate(randFloat());
  ASSERT_EQ(0, service.cam()
                   ->cam.camParameters.highFrequencyContainer.choice
                   .basicVehicleContainerHighFrequency.yawRate.yawRateValue);
}

TEST(CABasicService, SetConfiguration) {
  CABasicService service(getConverter());
  CABasicServiceConfig config;
  config.stationID = 1;
  config.stationType = StationType_PassengerCar;
  ASSERT_NO_THROW(service.configure(config));
  ASSERT_EQ(config.stationID, service.cam()->header.stationID);
  ASSERT_EQ(config.stationType,
            service.cam()->cam.camParameters.basicContainer.stationType);
  ASSERT_EQ(HighFrequencyContainer_PR_basicVehicleContainerHighFrequency,
            service.cam()->cam.camParameters.highFrequencyContainer.present);
}

TEST(CABasicService, GetConfiguration) {
  CABasicService service(getConverter());
  CABasicServiceConfig expectedConfig;
  expectedConfig.stationID = 1;
  expectedConfig.stationType = StationType_PassengerCar;
  service.configure(expectedConfig);
  auto config = service.getConfiguration();
  ASSERT_EQ(expectedConfig.stationID, config.stationID);
  ASSERT_EQ(expectedConfig.stationType, config.stationType);
  ASSERT_EQ(HighFrequencyContainer_PR_basicVehicleContainerHighFrequency,
            service.cam()->cam.camParameters.highFrequencyContainer.present);
}

TEST(CABasicService, ConfigureServiceAsRSU) {
  CABasicService service(getConverter());
  CABasicServiceConfig expectedConfig;
  expectedConfig.stationID = 1;
  expectedConfig.stationType = StationType_RoadSideUnit;
  service.configure(expectedConfig);
  ASSERT_EQ(HighFrequencyContainer_PR_rsuContainerHighFrequency,
            service.cam()->cam.camParameters.highFrequencyContainer.present);
}