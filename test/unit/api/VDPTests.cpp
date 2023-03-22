// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

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
  setHeading(value);
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
  setSpeed(value);
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
  setAcceleration(value);
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
  setYawRate(value);
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
  setCurvature(value);
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
  setDriveDirection(value);
  ASSERT_EQ(value, di::getCABasicService()
                       ->cam()
                       ->cam.camParameters.highFrequencyContainer.choice
                       .basicVehicleContainerHighFrequency.driveDirection);
}