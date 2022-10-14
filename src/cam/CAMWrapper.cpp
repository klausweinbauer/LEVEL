#include <CAMEncoder.hpp>
#include <CAMWrapper.hpp>
#include <Exception.hpp>
#include <vector>

namespace level::cam {

CAMDEREncoder CAMWrapper::_copyEncoder;

CAMWrapper::CAMWrapper() : CAMWrapper(0u) {}

CAMWrapper::CAMWrapper(CAM *cam) : _cam(cam) {}

CAMWrapper::CAMWrapper(unsigned int stationId)
    : CAMWrapper(stationId,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
}

CAMWrapper::CAMWrapper(unsigned int stationId, HighFrequencyContainer_PR type)
    : _cam((CAM *)calloc(1, sizeof(CAM))) {
  _cam->header.stationID = stationId;
  _cam->header.messageID = CAM_MESSAGE_ID;

  auto posConfEllipse = &_cam->cam.camParameters.basicContainer
                             .referencePosition.positionConfidenceEllipse;
  posConfEllipse->semiMajorConfidence = SemiAxisLength_unavailable;
  posConfEllipse->semiMinorConfidence = SemiAxisLength_unavailable;
  posConfEllipse->semiMajorOrientation = HeadingValue_unavailable;

  _cam->cam.camParameters.highFrequencyContainer.present = type;

  if (type == HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    initHFC();
  }
}

CAMWrapper::~CAMWrapper() { ASN_STRUCT_FREE(asn_DEF_CAM, _cam); }

CAMWrapper::CAMWrapper(const CAMWrapper &other) : _cam(nullptr) {
  try {
    auto data = _copyEncoder.encode(other._cam);
    _cam = _copyEncoder.decode(data);
  } catch (const Exception &e) {
    throw Exception(
        ERR, "Copying CAM failed. Only valid messages can be copied.\nError: " +
                 std::string(e.getErrMsg()));
  }
}

CAMWrapper &CAMWrapper::operator=(CAMWrapper other) {
  swap(*this, other);
  return *this;
}

CAM *CAMWrapper::operator->() const { return _cam; }

CAM &CAMWrapper::operator*() const { return *_cam; }

CAM *CAMWrapper::get() const { return _cam; }

LowFrequencyContainer *CAMWrapper::setLFC(LowFrequencyContainer_PR type) {
  if (type != LowFrequencyContainer_PR_NOTHING &&
      type != LowFrequencyContainer_PR_basicVehicleContainerLowFrequency) {
    throw Exception(ERR_INVALID_ARG, "Invalid low frequency container type.");
  }

  auto lfc = &_cam->cam.camParameters.lowFrequencyContainer;
  if (type == LowFrequencyContainer_PR_NOTHING) {
    clearLFC();
    return nullptr;
  } else if (*lfc != nullptr && (*lfc)->present == type) {
    return *lfc;
  }

  clearLFC();
  *lfc = (LowFrequencyContainer *)calloc(1, sizeof(LowFrequencyContainer));
  (*lfc)->present = type;
  return *lfc;
}

HighFrequencyContainer *CAMWrapper::setHFC(HighFrequencyContainer_PR type) {
  if (type != HighFrequencyContainer_PR_NOTHING &&
      type != HighFrequencyContainer_PR_basicVehicleContainerHighFrequency &&
      type != HighFrequencyContainer_PR_rsuContainerHighFrequency) {
    throw Exception(ERR, "Invalid high frequency container type.");
  }

  auto hfc = &_cam->cam.camParameters.highFrequencyContainer;
  if (hfc->present != type) {
    clearHFC();
    hfc->present = type;
    initHFC();
  }
  return hfc;
}

SpecialVehicleContainer *CAMWrapper::setSVC(SpecialVehicleContainer_PR type) {
  if (type != SpecialVehicleContainer_PR_NOTHING &&
      type != SpecialVehicleContainer_PR_dangerousGoodsContainer &&
      type != SpecialVehicleContainer_PR_emergencyContainer &&
      type != SpecialVehicleContainer_PR_publicTransportContainer &&
      type != SpecialVehicleContainer_PR_rescueContainer &&
      type != SpecialVehicleContainer_PR_roadWorksContainerBasic &&
      type != SpecialVehicleContainer_PR_safetyCarContainer &&
      type != SpecialVehicleContainer_PR_specialTransportContainer) {
    throw Exception(ERR, "Invalid special vehicle container type.");
  }

  auto svc = &_cam->cam.camParameters.specialVehicleContainer;
  if (type == SpecialVehicleContainer_PR_NOTHING) {
    clearSVC();
    return nullptr;
  } else if (*svc != nullptr && (*svc)->present == type) {
    return *svc;
  }

  clearSVC();
  *svc = (SpecialVehicleContainer *)calloc(1, sizeof(SpecialVehicleContainer));
  (*svc)->present = type;
  return *svc;
}

LowFrequencyContainer *CAMWrapper::getLFC() const {
  return _cam->cam.camParameters.lowFrequencyContainer;
}

HighFrequencyContainer *CAMWrapper::getHFC() const {
  return &_cam->cam.camParameters.highFrequencyContainer;
}

SpecialVehicleContainer *CAMWrapper::getSVC() const {
  return _cam->cam.camParameters.specialVehicleContainer;
}

void CAMWrapper::clearLFC() {
  auto lfc = &_cam->cam.camParameters.lowFrequencyContainer;
  ASN_STRUCT_FREE(asn_DEF_LowFrequencyContainer, *lfc);
  *lfc = nullptr;
}

void CAMWrapper::clearHFC() {
  HighFrequencyContainer hfc{};
  _cam->cam.camParameters.highFrequencyContainer = hfc;
}

void CAMWrapper::clearSVC() {
  auto svc = &_cam->cam.camParameters.specialVehicleContainer;
  ASN_STRUCT_FREE(asn_DEF_SpecialVehicleContainer, *svc);
  *svc = nullptr;
}

void CAMWrapper::initHFC() {
  if (_cam->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    auto hfc = &_cam->cam.camParameters.highFrequencyContainer.choice
                    .basicVehicleContainerHighFrequency;
    hfc->heading.headingConfidence = HeadingValue_unavailable;
    hfc->speed.speedConfidence = SpeedConfidence_unavailable;
    hfc->speed.speedConfidence = SpeedConfidence_unavailable;
    hfc->vehicleLength.vehicleLengthConfidenceIndication =
        VehicleLengthConfidenceIndication_unavailable;
    hfc->longitudinalAcceleration.longitudinalAccelerationConfidence =
        AccelerationConfidence_unavailable;
    hfc->curvature.curvatureConfidence = CurvatureConfidence_unavailable;
    hfc->yawRate.yawRateConfidence = YawRateConfidence_unavailable;
  }
}

int CAMWrapper::getBasicContainer(CAMBasicContainer_t *container) const {
  if (container) {
    auto bc = &_cam->cam.camParameters.basicContainer;
    container->stationType = (level::StationType)bc->stationType;
    container->latitude = bc->referencePosition.latitude;
    container->longitude = bc->referencePosition.longitude;
    container->altitude = bc->referencePosition.altitude.altitudeValue;
    return 0;
  } else {
    return ERR_ARG_NULL;
  }
}

int CAMWrapper::getBasicVehicleContainerHighFrequency(
    CAMBasicVehicleContainerHF_t *container) const {
  if (container) {
    auto hfcBase = &_cam->cam.camParameters.highFrequencyContainer;
    if (hfcBase->present ==
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
      auto hfc = &hfcBase->choice.basicVehicleContainerHighFrequency;
      container->headingValue = hfc->heading.headingValue;
      container->speedValue = hfc->speed.speedValue;
      container->driveDirection =
          (level::DriveDirectionType)hfc->driveDirection;
      container->vehicleLength = hfc->vehicleLength.vehicleLengthValue;
      container->vehicleWidth = hfc->vehicleWidth;
      container->longitudinalAccelerationValue =
          hfc->longitudinalAcceleration.longitudinalAccelerationValue;
      container->curvatureValue = hfc->curvature.curvatureValue;
      container->yawRateValue = hfc->yawRate.yawRateValue;
      return 0;
    } else {
      return ERR_HIGH_FREQ_CONTAINER_TYPE;
    }
  } else {
    return ERR_ARG_NULL;
  }
}

} // namespace level::cam