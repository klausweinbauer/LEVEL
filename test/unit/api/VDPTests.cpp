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

TEST(VDPTest, GetPointerToVDP) { ASSERT_NE(nullptr, getVDP()); }

TEST(VDPTest, SetHeading) {
  float value = randFloat();
  getVDP()->setHeading(value);
  ASSERT_EQ(converter.siToITSHeading(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.heading.headingValue);
}

TEST(VDPTest, SetSpeed) {
  float value = randFloat();
  getVDP()->setSpeed(value);
  ASSERT_EQ(converter.siToITSSpeed(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.speed.speedValue);
}

TEST(VDPTest, SetAcceleration) {
  float value = randFloat();
  getVDP()->setAcceleration(value);
  ASSERT_EQ(converter.siToITSLongitudinalAcceleration(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.longitudinalAcceleration
                .longitudinalAccelerationValue);
}

TEST(VDPTest, SetYawRate) {
  float value = randFloat();
  getVDP()->setYawRate(value);
  ASSERT_EQ(converter.siToITSYawRate(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.yawRate.yawRateValue);
}

TEST(VDPTest, SetCurvature) {
  float value = randFloat();
  getVDP()->setCurvature(value);
  ASSERT_EQ(converter.siToITSCurvature(value),
            di::getCABasicService()
                ->cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.curvature.curvatureValue);
}

TEST(VDPTest, SetDriveDirection) {
  auto value = DriveDirectionType::DirveDirection_Backward;
  getVDP()->setDriveDirection(value);
  ASSERT_EQ(value, di::getCABasicService()
                       ->cam()
                       ->cam.camParameters.highFrequencyContainer.choice
                       .basicVehicleContainerHighFrequency.driveDirection);
}