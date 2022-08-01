#include <CAMEncoder.hpp>
#include <CAMWrapper.hpp>
#include <Exception.hpp>
#include <vector>

namespace level::cam {

DEREncoder CAMWrapper::_copyEncoder;

CAMWrapper::CAMWrapper() : CAMWrapper(0) {}

CAMWrapper::CAMWrapper(int stationId)
    : CAMWrapper(stationId, HighFrequencyContainer_PR_NOTHING) {}

CAMWrapper::CAMWrapper(int stationId, HighFrequencyContainer_PR type)
    : _cam((CAM *)calloc(1, sizeof(CAM))) {
  _cam->header.stationID = stationId;
  _cam->cam.camParameters.highFrequencyContainer.present = type;
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

CAM *CAMWrapper::operator->() { return _cam; }
CAM &CAMWrapper::operator*() { return *_cam; }

LowFrequencyContainer *CAMWrapper::setLFC(LowFrequencyContainer_PR type) {
  auto lfc = &_cam->cam.camParameters.lowFrequencyContainer;
  if (type == LowFrequencyContainer_PR_NOTHING) {
    clearLFC();
    return nullptr;
  } else if (*lfc != nullptr && (*lfc)->present != type) {
    throw Exception(ERR, "Low frequency container already set.");
  }

  *lfc = (LowFrequencyContainer *)calloc(1, sizeof(LowFrequencyContainer));
  (*lfc)->present = type;
  return *lfc;
}

HighFrequencyContainer *CAMWrapper::setHFC(HighFrequencyContainer_PR type) {
  auto hfc = &_cam->cam.camParameters.highFrequencyContainer;
  if (type == HighFrequencyContainer_PR_NOTHING) {
    clearHFC();
  } else {
    if (hfc->present == HighFrequencyContainer_PR_NOTHING) {
      hfc->present = type;
    } else {
      throw Exception(ERR, "High frequency container already present.");
    }
  }
  return hfc;
}

SpecialVehicleContainer *CAMWrapper::setSVC(SpecialVehicleContainer_PR type) {
  auto svc = &_cam->cam.camParameters.specialVehicleContainer;
  if (type == SpecialVehicleContainer_PR_NOTHING) {
    clearSVC();
    return nullptr;
  } else if (*svc != nullptr && (*svc)->present != type) {
    throw Exception(ERR, "Special vehicle container already set.");
  }

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

} // namespace level::cam