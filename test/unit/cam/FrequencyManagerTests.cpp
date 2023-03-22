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

#include <IFrequencyManager.hpp>
#include <Mocks.hpp>
#include <ValueConverter.hpp>
#include <VehicleFrequencyManager.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::NiceMock;
using ::testing::Return;

namespace level::FrequencyManagerTest {

std::shared_ptr<NiceMock<MPOTI>> getPOTI() {
  return std::make_shared<NiceMock<MPOTI>>();
}

std::shared_ptr<cam::IFrequencyManager> getVehicleFM() {
  return std::make_shared<cam::VehicleFrequencyManager>(
      std::make_shared<ValueConverter>(), getPOTI());
}

} // namespace level::FrequencyManagerTest

using namespace level::FrequencyManagerTest;
using namespace level::cam;

TEST(FrequencyManager, VehicleFMGetTCheckCAMGen) {
  auto fm = getVehicleFM();
  ASSERT_GE(100, fm->getTCheckCAMGen());
}

TEST(FrequencyManager, VehicleFMCheckLowerBound) {
  auto fm = getVehicleFM();
  ASSERT_LE(100, fm->getTCAMGen());
}

TEST(FrequencyManager, VehicleFMCheckUpperBound) {
  auto fm = getVehicleFM();
  ASSERT_GE(1000, fm->getTCAMGen());
}

TEST(FrequencyManager, VehicleFMCAMGenInitialization) {
  auto fm = getVehicleFM();
  ASSERT_EQ(1000, fm->getTCAMGen());
}

TEST(FrequencyManager, VehicleFMCondition1Heading) {
  auto poti = getPOTI();
  unsigned long long int baseTime = 10000;
  EXPECT_CALL(*poti, now)
      .WillOnce(Return(baseTime))        // First notifyCAMGeneration()
      .WillOnce(Return(baseTime))        // First generateCAM()
      .WillOnce(Return(baseTime + 100)); // Second generateCAM()
  auto converter = std::make_shared<ValueConverter>();
  VehicleFrequencyManager fm(converter, poti);
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  auto headingValue =
      &cam->cam.camParameters.highFrequencyContainer.choice
           .basicVehicleContainerHighFrequency.heading.headingValue;
  *headingValue = converter->siToITSHeading(0);
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.generateCAM(cam));
  *headingValue = converter->siToITSHeading(4.1);
  ASSERT_TRUE(fm.generateCAM(cam));
}

TEST(FrequencyManager, VehicleFMCondition1Speed) {
  auto poti = getPOTI();
  unsigned long long int baseTime = 10000;
  EXPECT_CALL(*poti, now)
      .WillOnce(Return(baseTime))        // First notifyCAMGeneration()
      .WillOnce(Return(baseTime))        // First generateCAM()
      .WillOnce(Return(baseTime + 100)); // Second generateCAM()
  auto converter = std::make_shared<ValueConverter>();
  VehicleFrequencyManager fm(converter, poti);
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  auto speedValue = &cam->cam.camParameters.highFrequencyContainer.choice
                         .basicVehicleContainerHighFrequency.speed.speedValue;
  *speedValue = converter->siToITSSpeed(0);
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.generateCAM(cam));
  *speedValue = converter->siToITSSpeed(0.51);
  ASSERT_TRUE(fm.generateCAM(cam));
}

TEST(FrequencyManager, VehicleFMCondition1Position) {
  auto poti = getPOTI();
  unsigned long long int baseTime = 10000;
  EXPECT_CALL(*poti, now)
      .WillOnce(Return(baseTime))        // First notifyCAMGeneration()
      .WillOnce(Return(baseTime))        // First generateCAM()
      .WillOnce(Return(baseTime))        // Second notifyCAMGeneration()
      .WillOnce(Return(baseTime + 100)); // Second generateCAM()
  auto converter = std::make_shared<ValueConverter>();
  VehicleFrequencyManager fm(converter, poti);
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  auto refPos = &cam->cam.camParameters.basicContainer.referencePosition;
  refPos->latitude = converter->siToITSLatitude(47.05875);
  refPos->longitude = converter->siToITSLongitude(15.46134);
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.generateCAM(cam));
  fm.notifyCAMGeneration(cam);
  // Distance approximately 4.5m according to google maps
  refPos->latitude = converter->siToITSLatitude(47.05873);
  refPos->longitude = converter->siToITSLongitude(15.46129);
  ASSERT_TRUE(fm.generateCAM(cam));
}

TEST(FrequencyManager, TriggerConsecutiveCAMsAfterCondition1) {
  auto poti = getPOTI();
  unsigned long long int baseTime = 10000;
  EXPECT_CALL(*poti, now)
      .WillOnce(Return(baseTime))       // setup notifyCAMGeneration()
      .WillOnce(Return(baseTime + 100)) // first generateCAM()
      .WillOnce(Return(baseTime + 100)) // first notifyCAMGeneration()
      .WillOnce(Return(baseTime + 200)) // second generateCAM()
      .WillOnce(Return(baseTime + 200)) // second notifyCAMGeneration()
      .WillOnce(Return(baseTime + 300)) // third generateCAM()
      .WillOnce(Return(baseTime + 300)) // third notifyCAMGeneration()
      .WillOnce(Return(baseTime + 1299))
      .WillOnce(Return(baseTime + 1300));
  auto converter = std::make_shared<ValueConverter>();
  VehicleFrequencyManager fm(converter, poti);
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  auto headingValue =
      &cam->cam.camParameters.highFrequencyContainer.choice
           .basicVehicleContainerHighFrequency.heading.headingValue;
  *headingValue = converter->siToITSHeading(0);
  fm.notifyCAMGeneration(cam);
  *headingValue = converter->siToITSHeading(4.1);
  ASSERT_TRUE(fm.generateCAM(cam));
  fm.notifyCAMGeneration(cam);
  ASSERT_TRUE(fm.generateCAM(cam));
  fm.notifyCAMGeneration(cam);
  ASSERT_TRUE(fm.generateCAM(cam));
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.generateCAM(cam));
  ASSERT_TRUE(fm.generateCAM(cam));
}

TEST(FrequencyManager, VehicleFMCondition2) {
  auto poti = getPOTI();
  unsigned long long int baseTime = 10000;
  EXPECT_CALL(*poti, now)
      .WillOnce(Return(baseTime))         // First notifyCAMGeneration()
      .WillOnce(Return(baseTime + 900))   // First generateCAM()
      .WillOnce(Return(baseTime + 1000)); // Second generateCAM()
  auto converter = std::make_shared<ValueConverter>();
  VehicleFrequencyManager fm(converter, poti);
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.generateCAM(cam));
  ASSERT_TRUE(fm.generateCAM(cam));
}

TEST(FrequencyManager, VehicleFMCondition1) {
  auto poti = getPOTI();
  unsigned long long int baseTime = 10000;
  EXPECT_CALL(*poti, now)
      .WillOnce(Return(baseTime))         // First notifyCAMGeneration()
      .WillOnce(Return(baseTime + 900))   // First generateCAM()
      .WillOnce(Return(baseTime + 1000)); // Second generateCAM()
  auto converter = std::make_shared<ValueConverter>();
  VehicleFrequencyManager fm(converter, poti);
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.generateCAM(cam));
  ASSERT_TRUE(fm.generateCAM(cam));
}

TEST(FrequencyManager, VehicleFMCondition1SetTGenCAM) {
  auto poti = getPOTI();
  unsigned long long int baseTime = 10000;
  EXPECT_CALL(*poti, now)
      .WillOnce(Return(baseTime))
      .WillOnce(Return(baseTime + 200))
      .WillOnce(Return(baseTime + 200))
      .WillOnce(Return(baseTime + 399))
      .WillOnce(Return(baseTime + 400));
  auto converter = std::make_shared<ValueConverter>();
  VehicleFrequencyManager fm(converter, poti);
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  auto headingValue =
      &cam->cam.camParameters.highFrequencyContainer.choice
           .basicVehicleContainerHighFrequency.heading.headingValue;
  *headingValue = converter->siToITSHeading(0);
  fm.notifyCAMGeneration(cam);
  *headingValue = converter->siToITSHeading(4.1);
  ASSERT_TRUE(fm.generateCAM(cam));
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.generateCAM(cam));
  ASSERT_TRUE(fm.generateCAM(cam));
}

TEST(FrequencyManager, VehicleFMIncludeLFC) {
  auto poti = getPOTI();
  unsigned long long int baseTime = 10000;
  EXPECT_CALL(*poti, now)
      .WillOnce(Return(baseTime))
      .WillOnce(Return(baseTime))
      .WillOnce(Return(baseTime + 499))
      .WillOnce(Return(baseTime + 500));
  auto converter = std::make_shared<ValueConverter>();
  VehicleFrequencyManager fm(converter, poti);
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  cam.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  ASSERT_TRUE(fm.includeLFC());
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.includeLFC());
  ASSERT_TRUE(fm.includeLFC());
}

TEST(FrequencyManager, VehicleFMIncludeLFCAfterMultipleMessages) {
  auto poti = getPOTI();
  unsigned long long int baseTime = 10000;
  EXPECT_CALL(*poti, now)
      .WillOnce(Return(baseTime))
      .WillOnce(Return(baseTime))
      .WillOnce(Return(baseTime + 200))
      .WillOnce(Return(baseTime + 200))
      .WillOnce(Return(baseTime + 400))
      .WillOnce(Return(baseTime + 400))
      .WillOnce(Return(baseTime + 600))
      .WillOnce(Return(baseTime + 600));
  auto converter = std::make_shared<ValueConverter>();
  VehicleFrequencyManager fm(converter, poti);
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  cam.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  fm.notifyCAMGeneration(cam);
  cam.clearLFC();
  ASSERT_FALSE(fm.includeLFC());
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.includeLFC());
  fm.notifyCAMGeneration(cam);
  ASSERT_FALSE(fm.includeLFC());
  fm.notifyCAMGeneration(cam);
  ASSERT_TRUE(fm.includeLFC());
}