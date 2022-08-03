#include <CAMWrapper.hpp>
#include <Exception.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;
using namespace level::cam;

TEST(CAMWrapper, SuccessfulConstruction) {
  CAMWrapper cam(1);
  ASSERT_EQ(1, cam->header.stationID);
  ASSERT_EQ(CAM_MESSAGE_ID, cam->header.messageID);

  auto posConfEllipse = &cam->cam.camParameters.basicContainer.referencePosition
                             .positionConfidenceEllipse;
  ASSERT_EQ(SemiAxisLength_unavailable, posConfEllipse->semiMajorConfidence);
  ASSERT_EQ(SemiAxisLength_unavailable, posConfEllipse->semiMinorConfidence);
  ASSERT_EQ(HeadingValue_unavailable, posConfEllipse->semiMajorOrientation);
}

TEST(CAMWrapper, SuccessfulConstructionForVehicle) {
  CAMWrapper cam(0,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);

  auto hfc = &cam->cam.camParameters.highFrequencyContainer.choice
                  .basicVehicleContainerHighFrequency;
  ASSERT_EQ(HeadingValue_unavailable, hfc->heading.headingConfidence);
  ASSERT_EQ(SpeedConfidence_unavailable, hfc->speed.speedConfidence);
  ASSERT_EQ(SpeedConfidence_unavailable, hfc->speed.speedConfidence);
  ASSERT_EQ(VehicleLengthConfidenceIndication_unavailable,
            hfc->vehicleLength.vehicleLengthConfidenceIndication);
  ASSERT_EQ(AccelerationConfidence_unavailable,
            hfc->longitudinalAcceleration.longitudinalAccelerationConfidence);
  ASSERT_EQ(CurvatureConfidence_unavailable,
            hfc->curvature.curvatureConfidence);
  ASSERT_EQ(YawRateConfidence_unavailable, hfc->yawRate.yawRateConfidence);
}

TEST(CAMWrapper, InitializeWithPointer) {
  int id = rand();
  CAM *camPtr = (CAM *)calloc(1, sizeof(CAM));
  camPtr->header.messageID = id;
  CAMWrapper cam(camPtr);
  ASSERT_EQ(id, cam->header.messageID);
}

TEST(CAMWrapper, ArrowOperator) {
  CAMWrapper cam(1);
  cam->header.stationID = 2;
  ASSERT_EQ(2, cam->header.stationID);
}

TEST(CAMWrapper, PointerOperator) {
  CAMWrapper cam(1);
  (*cam).header.stationID = 2;
  ASSERT_EQ(2, (*cam).header.stationID);
}

TEST(CAMWrapper, GetCAM) {
  int id = rand();
  CAMWrapper cam(id);
  ASSERT_EQ(id, cam.get()->header.stationID);
}

TEST(CAMWrapper, FailsOnCopyingInvalidMessage) {
  CAMWrapper cam1(1);
  cam1->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_NOTHING;
  ASSERT_THROW(CAMWrapper cam2(cam1), Exception);
}

TEST(CAMWrapper, CopyConstructor) {
  CAMWrapper cam1(0, HighFrequencyContainer_PR_rsuContainerHighFrequency);
  cam1.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  CAMWrapper cam2(cam1);
  ASSERT_NE(nullptr, cam1.getLFC());
  ASSERT_NE(nullptr, cam2.getLFC());
  ASSERT_NE(cam2.getLFC(), cam1.getLFC());
}

TEST(CAMWrapper, AssignmentOperator) {
  CAMWrapper cam1(0, HighFrequencyContainer_PR_rsuContainerHighFrequency);
  cam1.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  CAMWrapper cam2(0, HighFrequencyContainer_PR_rsuContainerHighFrequency);
  ASSERT_EQ(nullptr, cam2.getLFC());
  cam2 = cam1;
  ASSERT_NE(nullptr, cam1.getLFC());
  ASSERT_NE(nullptr, cam2.getLFC());
  ASSERT_NE(cam2.getLFC(), cam1.getLFC());
}

TEST(CAMWrapper, GetHFC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto hfc = cam.getHFC();
  ASSERT_NE(nullptr, hfc);
  ASSERT_EQ(HighFrequencyContainer_PR_NOTHING, hfc->present);
  ASSERT_EQ(&cam->cam.camParameters.highFrequencyContainer, hfc);
}

TEST(CAMWrapper, SetHFC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto type = HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  auto hfc = cam.setHFC(type);
  ASSERT_EQ(type, hfc->present);

  auto hfcbv = &hfc->choice.basicVehicleContainerHighFrequency;
  ASSERT_EQ(HeadingValue_unavailable, hfcbv->heading.headingConfidence);
  ASSERT_EQ(SpeedConfidence_unavailable, hfcbv->speed.speedConfidence);
  ASSERT_EQ(SpeedConfidence_unavailable, hfcbv->speed.speedConfidence);
  ASSERT_EQ(VehicleLengthConfidenceIndication_unavailable,
            hfcbv->vehicleLength.vehicleLengthConfidenceIndication);
  ASSERT_EQ(AccelerationConfidence_unavailable,
            hfcbv->longitudinalAcceleration.longitudinalAccelerationConfidence);
  ASSERT_EQ(CurvatureConfidence_unavailable,
            hfcbv->curvature.curvatureConfidence);
  ASSERT_EQ(YawRateConfidence_unavailable, hfcbv->yawRate.yawRateConfidence);
}

TEST(CAMWrapper, ResetHFC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  cam.setHFC(HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  ASSERT_THROW(cam.setHFC(HighFrequencyContainer_PR_rsuContainerHighFrequency),
               Exception);
}

TEST(CAMWrapper, SetHFCToNoting) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto type = HighFrequencyContainer_PR_NOTHING;
  cam.setHFC(HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  ASSERT_NE(nullptr, cam.setHFC(type));
  ASSERT_EQ(type, cam.getHFC()->present);
}

TEST(CAMWrapper, ClearHFC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  cam.setHFC(HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
  ASSERT_NO_THROW(cam.clearHFC());
  ASSERT_EQ(HighFrequencyContainer_PR_NOTHING,
            cam->cam.camParameters.highFrequencyContainer.present);
}

TEST(CAMWrapper, ClearHFCValues) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto type = HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  int speed = rand();
  auto hfc = cam.setHFC(type);
  hfc->choice.basicVehicleContainerHighFrequency.speed.speedValue = speed;
  cam.clearHFC();
  hfc = cam.setHFC(type);
  ASSERT_EQ(0, hfc->choice.basicVehicleContainerHighFrequency.speed.speedValue);
}

TEST(CAMWrapper, ClearHFCUninitialized) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  ASSERT_NO_THROW(cam.clearHFC());
}

TEST(CAMWrapper, GetLFCUninitialized) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  ASSERT_EQ(nullptr, cam.getLFC());
}

TEST(CAMWrapper, GetLFC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto lfc = (LowFrequencyContainer *)calloc(1, sizeof(LowFrequencyContainer));
  cam->cam.camParameters.lowFrequencyContainer = lfc;
  ASSERT_EQ(lfc, cam.getLFC());
}

TEST(CAMWrapper, SetLFC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto type = LowFrequencyContainer_PR_basicVehicleContainerLowFrequency;
  auto lfc = cam.setLFC(type);
  ASSERT_NE(nullptr, lfc);
  ASSERT_EQ(type, lfc->present);
  ASSERT_EQ(type, cam->cam.camParameters.lowFrequencyContainer->present);
}

TEST(CAMWrapper, SetLFCToNoting) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  cam.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  ASSERT_EQ(nullptr, cam.setLFC(LowFrequencyContainer_PR_NOTHING));
  ASSERT_EQ(nullptr, cam->cam.camParameters.lowFrequencyContainer);
}

TEST(CAMWrapper, ResetLFC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  cam.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  ASSERT_THROW(cam.setLFC((LowFrequencyContainer_PR)2), Exception);
}

TEST(CAMWrapper, ClearLFC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  cam.setLFC(LowFrequencyContainer_PR_basicVehicleContainerLowFrequency);
  ASSERT_NO_THROW(cam.clearLFC());
  ASSERT_EQ(nullptr, cam->cam.camParameters.lowFrequencyContainer);
}

TEST(CAMWrapper, ClearLFCUninitialized) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  ASSERT_NO_THROW(cam.clearLFC());
}

TEST(CAMWrapper, ClearLFCValues) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto type = LowFrequencyContainer_PR_basicVehicleContainerLowFrequency;
  auto lfc1 = cam.setLFC(type);
  lfc1->choice.basicVehicleContainerLowFrequency.vehicleRole = rand();
  cam.clearLFC();
  auto lfc2 = cam.setLFC(type);
  ASSERT_EQ(0, lfc2->choice.basicVehicleContainerLowFrequency.vehicleRole);
}

TEST(CAMWrapper, GetSVCUninitialized) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  ASSERT_EQ(nullptr, cam.getSVC());
}

TEST(CAMWrapper, GetSVC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto svc =
      (SpecialVehicleContainer *)calloc(1, sizeof(SpecialVehicleContainer));
  cam->cam.camParameters.specialVehicleContainer = svc;
  ASSERT_EQ(svc, cam.getSVC());
}

TEST(CAMWrapper, SetSVC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto type = SpecialVehicleContainer_PR_dangerousGoodsContainer;
  auto svc = cam.setSVC(type);
  ASSERT_NE(nullptr, svc);
  ASSERT_EQ(type, svc->present);
  ASSERT_EQ(type, cam->cam.camParameters.specialVehicleContainer->present);
}

TEST(CAMWrapper, SetSVCToNoting) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  cam.setSVC(SpecialVehicleContainer_PR_dangerousGoodsContainer);
  ASSERT_EQ(nullptr, cam.setSVC(SpecialVehicleContainer_PR_NOTHING));
  ASSERT_EQ(nullptr, cam->cam.camParameters.specialVehicleContainer);
}

TEST(CAMWrapper, ResetSVC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  cam.setSVC(SpecialVehicleContainer_PR_dangerousGoodsContainer);
  ASSERT_THROW(cam.setSVC(SpecialVehicleContainer_PR_emergencyContainer),
               Exception);
}

TEST(CAMWrapper, ClearSVC) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  cam.setSVC(SpecialVehicleContainer_PR_emergencyContainer);
  ASSERT_NO_THROW(cam.clearSVC());
  ASSERT_EQ(nullptr, cam->cam.camParameters.specialVehicleContainer);
}

TEST(CAMWrapper, ClearSVCUninitialized) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  ASSERT_NO_THROW(cam.clearSVC());
}

TEST(CAMWrapper, ClearSVCValues) {
  CAMWrapper cam(0, HighFrequencyContainer_PR_NOTHING);
  auto type = SpecialVehicleContainer_PR_dangerousGoodsContainer;
  auto svc1 = cam.setSVC(type);
  svc1->choice.dangerousGoodsContainer.dangerousGoodsBasic = rand();
  cam.clearSVC();
  auto svc2 = cam.setSVC(type);
  ASSERT_EQ(0, svc2->choice.dangerousGoodsContainer.dangerousGoodsBasic);
}