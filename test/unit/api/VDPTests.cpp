#include <Mocks.hpp>
#include <ServiceProvider.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <level.h>
#include <mutex>

using ::testing::_;
using ::testing::NiceMock;

namespace level::VDPTests {

ValueConverter converter;
float randFloat() { return rand() * 3.1415962535; }

} // namespace level::VDPTests

using namespace level::VDPTests;
using namespace level;

TEST(VDPTest, SetHeading) {
  CABasicServiceConfig config = getCABasicServiceConfig();
  config.stationType = StationType_PassengerCar;
  configureCABasicService(config);

  float value = randFloat();
  VDP_setHeading(value);
  ASSERT_EQ(converter.siToITSHeading(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.heading.headingValue);
}

TEST(VDPTest, SetSpeed) {
  CABasicServiceConfig config = getCABasicServiceConfig();
  config.stationType = StationType_PassengerCar;
  configureCABasicService(config);

  float value = randFloat();
  VDP_setSpeed(value);
  ASSERT_EQ(converter.siToITSSpeed(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.speed.speedValue);
}

TEST(VDPTest, SetAcceleration) {
  CABasicServiceConfig config = getCABasicServiceConfig();
  config.stationType = StationType_PassengerCar;
  configureCABasicService(config);

  float value = randFloat();
  VDP_setAcceleration(value);
  ASSERT_EQ(converter.siToITSLongitudinalAcceleration(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.longitudinalAcceleration
                .longitudinalAccelerationValue);
}

TEST(VDPTest, SetYawRate) {
  CABasicServiceConfig config = getCABasicServiceConfig();
  config.stationType = StationType_PassengerCar;
  configureCABasicService(config);

  float value = randFloat();
  VDP_setYawRate(value);
  ASSERT_EQ(converter.siToITSYawRate(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.yawRate.yawRateValue);
}

TEST(VDPTest, SetCurvature) {
  CABasicServiceConfig config = getCABasicServiceConfig();
  config.stationType = StationType_PassengerCar;
  configureCABasicService(config);

  float value = randFloat();
  VDP_setCurvature(value);
  ASSERT_EQ(converter.siToITSCurvature(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.curvature.curvatureValue);
}

TEST(VDPTest, SetDriveDirection) {
  CABasicServiceConfig config = getCABasicServiceConfig();
  config.stationType = StationType_PassengerCar;
  configureCABasicService(config);

  auto value = DriveDirectionType::DirveDirection_Backward;
  VDP_setDriveDirection(value);
  ASSERT_EQ(value, di::getCABasicService()
                       ->cam()
                       ->cam.camParameters.highFrequencyContainer.choice
                       .basicVehicleContainerHighFrequency.driveDirection);
}