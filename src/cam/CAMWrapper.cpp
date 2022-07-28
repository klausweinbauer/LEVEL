#include <CAMWrapper.hpp>
#include <Exception.hpp>
#include <vector>

namespace level {
namespace cam {

CAMWrapper::CAMWrapper(int stationId)
    : CAMWrapper(stationId,
                 HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
}

CAMWrapper::CAMWrapper(int stationId, HighFrequencyContainer_PR type)
    : _cam((CAM_t *)calloc(1, sizeof(CAM_t))) {
  _cam->header.stationID = stationId;
  _cam->cam.camParameters.highFrequencyContainer.present = type;
}

CAMWrapper::~CAMWrapper() { ASN_STRUCT_FREE(asn_DEF_CAM, _cam); }

static int writeCallback(const void *src, size_t size,
                         void *application_specific_key) {

  auto buffer = (std::vector<uint8_t> *)application_specific_key;

  for (size_t i = 0; i < size; i++) {
    uint8_t data = ((uint8_t *)src)[i];
    buffer->push_back(data);
  }

  return 0;
}

CAMWrapper::CAMWrapper(const CAMWrapper &other) : _cam(nullptr) {
  std::vector<uint8_t> dynamicBuffer;
  asn_enc_rval_t retValEncode;
  retValEncode = der_encode(&asn_DEF_CAM, (void *)other._cam, writeCallback,
                            (void *)&dynamicBuffer);

  if (retValEncode.encoded == -1) {
    std::stringstream errMsgStream;
    errMsgStream << "Copying CAM failed during encoding (Code="
                 << retValEncode.failed_type->name << "). "
                 << "This is probably due to an invalid value of property '"
                 << retValEncode.failed_type->xml_tag << "'." << std::endl;
    throw Exception(ERR_ENCODE, errMsgStream.str());
  }

  const size_t bufferLen = dynamicBuffer.size();
  uint8_t *buffer = (uint8_t *)calloc(bufferLen, sizeof(uint8_t));
  std::copy(dynamicBuffer.begin(), dynamicBuffer.end(), buffer);

  asn_dec_rval_t retValDecode;
  asn_codec_ctx_t opt_codec_ctx{};
  opt_codec_ctx.max_stack_size = 0;
  retValDecode = ber_decode(&opt_codec_ctx, &asn_DEF_CAM, (void **)&_cam,
                            buffer, bufferLen);

  free(buffer);

  if (retValDecode.code != asn_dec_rval_code_e::RC_OK) {
    ASN_STRUCT_FREE(asn_DEF_CAM, _cam);
    throw Exception(ERR_DECODE, "Copying CAM failed during decoding.");
  }
}

CAMWrapper &CAMWrapper::operator=(CAMWrapper other) {
  swap(*this, other);
  return *this;
}

CAM_t *CAMWrapper::operator->() { return _cam; }

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

} // namespace cam
} // namespace level