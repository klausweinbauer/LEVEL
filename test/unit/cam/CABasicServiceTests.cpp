#include <CABasicService.hpp>
#include <Mocks.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

namespace level::CABasicServiceTests {

float randFloat() { return rand() * 3.1415962535; }

CABasicServiceConfig setTestConfig = {0, StationType_PassengerCar};

std::shared_ptr<NiceMock<MValueConverter>> getConverter() {
  return std::make_shared<NiceMock<MValueConverter>>();
}

std::shared_ptr<NiceMock<MNetworkInterface<CAM>>> getNAL() {
  return std::make_shared<NiceMock<MNetworkInterface<CAM>>>();
}

std::shared_ptr<NiceMock<MFrequencyManager>> getFM() {
  auto fm = std::make_shared<NiceMock<MFrequencyManager>>();
  ON_CALL(*fm, getTCheckCAMGen()).WillByDefault(Return(0));
  return fm;
}

std::shared_ptr<NiceMock<MPOTI>> getPOTI() {
  return std::make_shared<NiceMock<MPOTI>>();
}

} // namespace level::CABasicServiceTests

using namespace level::cam;
using namespace level::CABasicServiceTests;

TEST(CABasicService, SetHeading) {
  auto converter = getConverter();
  float siValue = randFloat();
  int itsValue = rand();
  EXPECT_CALL(*converter, siToITSHeading(siValue)).WillOnce(Return(itsValue));
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  service.configure(setTestConfig);
  service.setHeading(siValue);
  EXPECT_EQ(itsValue,
            service.cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.heading.headingValue);
}

TEST(CABasicService, IgnoreSetHeadingForInvalidHFC) {
  auto converter = getConverter();
  EXPECT_CALL(*converter, siToITSHeading(_)).Times(0);
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  CABasicServiceConfig config;
  config.stationType = StationType_RoadSideUnit;
  service.configure(config);
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
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  service.configure(setTestConfig);
  service.setSpeed(siValue);
  EXPECT_EQ(itsValue, service.cam()
                          ->cam.camParameters.highFrequencyContainer.choice
                          .basicVehicleContainerHighFrequency.speed.speedValue);
}

TEST(CABasicService, IgnoreSetSpeedForInvalidHFC) {
  auto converter = getConverter();
  EXPECT_CALL(*converter, siToITSSpeed(_)).Times(0);
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  CABasicServiceConfig config;
  config.stationType = StationType_RoadSideUnit;
  service.configure(config);
  service.setSpeed(randFloat());
  ASSERT_EQ(0, service.cam()
                   ->cam.camParameters.highFrequencyContainer.choice
                   .basicVehicleContainerHighFrequency.speed.speedValue);
}

TEST(CABasicService, SetDriveDirection) {
  auto converter = getConverter();
  DriveDirectionType direction = DriveDirectionType::DirveDirection_Backward;
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  service.configure(setTestConfig);
  service.setDriveDirection(direction);
  EXPECT_EQ(direction, service.cam()
                           ->cam.camParameters.highFrequencyContainer.choice
                           .basicVehicleContainerHighFrequency.driveDirection);
}

TEST(CABasicService, IgnoreSetDirectionForInvalidHFC) {
  auto converter = getConverter();
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  CABasicServiceConfig config;
  config.stationType = StationType_RoadSideUnit;
  service.configure(config);
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
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  service.configure(setTestConfig);
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
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  CABasicServiceConfig config;
  config.stationType = StationType_RoadSideUnit;
  service.configure(config);
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
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  service.configure(setTestConfig);
  service.setCurvature(siValue);
  EXPECT_EQ(itsValue,
            service.cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.curvature.curvatureValue);
}

TEST(CABasicService, IgnoreSetCurvatureForInvalidHFC) {
  auto converter = getConverter();
  EXPECT_CALL(*converter, siToITSCurvature(_)).Times(0);
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  CABasicServiceConfig config;
  config.stationType = StationType_RoadSideUnit;
  service.configure(config);
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
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  service.configure(setTestConfig);
  service.setYawRate(siValue);
  EXPECT_EQ(itsValue,
            service.cam()
                ->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.yawRate.yawRateValue);
}

TEST(CABasicService, IgnoreSetYawRateForInvalidHFC) {
  auto converter = getConverter();
  EXPECT_CALL(*converter, siToITSYawRate(_)).Times(0);
  CABasicService service(getNAL(), converter, getFM(), getPOTI());
  CABasicServiceConfig config;
  config.stationType = StationType_RoadSideUnit;
  service.configure(config);
  service.setYawRate(randFloat());
  ASSERT_EQ(0, service.cam()
                   ->cam.camParameters.highFrequencyContainer.choice
                   .basicVehicleContainerHighFrequency.yawRate.yawRateValue);
}

TEST(CABasicService, SetConfiguration) {
  CABasicService service(getNAL(), getConverter(), getFM(), getPOTI());
  CABasicServiceConfig config;
  config.stationID = 1;
  config.stationType = StationType_PassengerCar;
  ASSERT_NO_THROW(service.configure(config));
  ASSERT_EQ(config.stationID, service.cam()->header.stationID);
  ASSERT_EQ(config.stationType,
            service.cam()->cam.camParameters.basicContainer.stationType);
  ASSERT_EQ(HighFrequencyContainer_PR_basicVehicleContainerHighFrequency,
            service.cam()->cam.camParameters.highFrequencyContainer.present);
  ASSERT_NE(nullptr, service.cam()->cam.camParameters.lowFrequencyContainer);
  ASSERT_EQ(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency,
            service.cam()->cam.camParameters.lowFrequencyContainer->present);
}

TEST(CABasicService, GetConfiguration) {
  CABasicService service(getNAL(), getConverter(), getFM(), getPOTI());
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
  CABasicService service(getNAL(), getConverter(), getFM(), getPOTI());
  CABasicServiceConfig expectedConfig;
  expectedConfig.stationID = 1;
  expectedConfig.stationType = StationType_RoadSideUnit;
  service.configure(expectedConfig);
  ASSERT_EQ(HighFrequencyContainer_PR_rsuContainerHighFrequency,
            service.cam()->cam.camParameters.highFrequencyContainer.present);
  ASSERT_EQ(nullptr, service.cam()->cam.camParameters.lowFrequencyContainer);
}

TEST(CABasicService, RunDisseminationThread) {
  auto fm = getFM();
  bool waitFlag = true;

  EXPECT_CALL(*fm, getTCheckCAMGen())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(1));
  EXPECT_CALL(*fm, generateCAM(_))
      .Times(AtLeast(1))
      .WillRepeatedly(Invoke([&waitFlag](const CAMWrapper &cam) {
        waitFlag = false;
        return false;
      }));
  {
    CABasicService service(getNAL(), getConverter(), fm, getPOTI());
    while (waitFlag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}

TEST(CABasicService, PassGeneratedCAMToNetwork) {
  auto fm = getFM();
  auto nal = getNAL();
  bool waitFlag = true;
  bool hasLFC = true;
  const CAM *fmCAM;
  const CAM *nalCAM;

  EXPECT_CALL(*fm, getTCheckCAMGen()).WillRepeatedly(Return(1));
  EXPECT_CALL(*fm, includeLFC()).WillOnce(Return(false));
  EXPECT_CALL(*fm, notifyCAMGeneration(_))
      .WillOnce(Invoke([&fmCAM](const CAMWrapper &cam) { fmCAM = cam.get(); }));
  EXPECT_CALL(*fm, generateCAM(_))
      .WillOnce(Return(true))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(*nal, send(_))
      .WillOnce(Invoke([&nalCAM, &waitFlag, &hasLFC](const CAM *cam) {
        nalCAM = cam;
        waitFlag = false;
        hasLFC = cam->cam.camParameters.lowFrequencyContainer != nullptr;
        return true;
      }));

  {
    CABasicServiceConfig config;
    config.stationID = 0;
    config.stationType = StationType_PassengerCar;
    CABasicService service(nal, getConverter(), fm, getPOTI());
    service.configure(config);
    while (waitFlag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  ASSERT_EQ(fmCAM, nalCAM);
  ASSERT_FALSE(hasLFC);
}

TEST(CABasicService, PassGeneratedCAMToNetworkWithLFC) {
  auto fm = getFM();
  auto nal = getNAL();
  bool waitFlag = true;
  bool hasLFC = false;

  EXPECT_CALL(*fm, getTCheckCAMGen()).WillRepeatedly(Return(1));
  EXPECT_CALL(*fm, includeLFC()).WillRepeatedly(Return(true));
  EXPECT_CALL(*fm, notifyCAMGeneration(_)).Times(AtLeast(1));
  EXPECT_CALL(*fm, generateCAM(_))
      .WillOnce(Return(true))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(*nal, send(_))
      .WillOnce(Invoke([&waitFlag, &hasLFC](const CAM *cam) {
        waitFlag = false;
        hasLFC = cam->cam.camParameters.lowFrequencyContainer != nullptr;
        return true;
      }));

  {
    CABasicServiceConfig config;
    config.stationID = 0;
    config.stationType = StationType_PassengerCar;
    CABasicService service(nal, getConverter(), fm, getPOTI());
    service.configure(config);
    while (waitFlag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  ASSERT_TRUE(hasLFC);
}

TEST(CABasicService, CheckGenDeltaTimeForGeneratedMessage) {
  auto fm = getFM();
  auto nal = getNAL();
  auto poti = getPOTI();
  bool waitFlag = true;
  int genDeltaTime = 0;
  const unsigned long long int deltaTime = 10000;
  const unsigned long long int timestamp = 2 * 65536 + deltaTime;

  EXPECT_CALL(*poti, now()).Times(AtLeast(1)).WillRepeatedly(Return(timestamp));

  EXPECT_CALL(*fm, getTCheckCAMGen()).WillRepeatedly(Return(1));
  EXPECT_CALL(*fm, includeLFC()).WillRepeatedly(Return(true));
  EXPECT_CALL(*fm, notifyCAMGeneration(_)).Times(AtLeast(1));
  EXPECT_CALL(*fm, generateCAM(_))
      .WillOnce(Return(true))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(*nal, send(_))
      .WillOnce(Invoke([&waitFlag, &genDeltaTime](const CAM *cam) {
        waitFlag = false;
        genDeltaTime = cam->cam.generationDeltaTime;
        return true;
      }));

  {
    CABasicServiceConfig config;
    config.stationID = 0;
    config.stationType = StationType_PassengerCar;
    CABasicService service(nal, std::make_shared<ValueConverter>(), fm, poti);
    service.configure(config);
    while (waitFlag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  ASSERT_EQ(deltaTime, genDeltaTime);
}

TEST(CABasicService, GetCAMGenerationFrequency) {
  int tGen = 200;
  auto fm = getFM();
  auto valConv = getConverter();
  auto nal = getNAL();
  auto poti = getPOTI();
  EXPECT_CALL(*fm, getTCAMGen).WillOnce(Return(tGen));
  CABasicService service(nal, valConv, fm, poti);
  ASSERT_EQ(5.0, service.getCAMGenerationFrequency());
}