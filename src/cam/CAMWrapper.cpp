#include <CAMWrapper.hpp>
#include <DEREncoder.hpp>
#include <Exception.hpp>
#include <vector>

namespace level::cam {

std::shared_ptr<IEncoder<CAM>> CAMWrapper::copyEncoder =
    std::make_shared<DEREncoder<CAM>>(asn_DEF_CAM);

CAMWrapper::CAMWrapper(int stationId)
    : CAMWrapper(stationId,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
}

CAMWrapper::CAMWrapper(int stationId, HighFrequencyContainer_PR type)
    : _cam((CAM *)calloc(1, sizeof(CAM))) {
  _cam->header.stationID = stationId;
  _cam->cam.camParameters.highFrequencyContainer.present = type;
}

CAMWrapper::~CAMWrapper() { ASN_STRUCT_FREE(asn_DEF_CAM, _cam); }

CAMWrapper::CAMWrapper(const CAMWrapper &other) : _cam(nullptr) {
  auto safeEncoder = copyEncoder;
  if (!safeEncoder) {
    throw Exception(ERR, "No copy encoder provided. Set a copy encoder with "
                         "level::cam::CAMWrapper::copyEncoder.");
  }

  try {
    auto data = safeEncoder->encode(other._cam);
    _cam = safeEncoder->decode(data);
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

void CAMWrapper::setLFC(LowFrequencyContainer_PR type) {
  auto lfcPtr = &_cam->cam.camParameters.lowFrequencyContainer;

  if (*lfcPtr != nullptr) {
    ASN_STRUCT_FREE(asn_DEF_LowFrequencyContainer, *lfcPtr);
  }

  if (type == LowFrequencyContainer_PR::LowFrequencyContainer_PR_NOTHING) {
    *lfcPtr = nullptr;
  } else {
    auto lfc =
        (LowFrequencyContainer *)calloc(1, sizeof(LowFrequencyContainer));
    lfc->present = type;
    *lfcPtr = lfc;
  }
}

} // namespace level::cam