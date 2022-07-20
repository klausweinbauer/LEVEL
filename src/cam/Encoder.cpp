#include <Encoder.hpp>
#include <Exception.hpp>
#include <vector>

namespace level {
  namespace cam {

Encoder::Encoder() {}

Encoder::~Encoder() {}

static int writeCallback(const void *src, size_t size,
                         void *application_specific_key) {

  auto buffer = (std::vector<uint8_t> *)application_specific_key;

  for (size_t i = 0; i < size; i++) {
    uint8_t data = ((uint8_t *)src)[i];
    buffer->push_back(data);
  }

  return 0;
}

int Encoder::encode(const CAM_t *cam, uint8_t *buffer, int bufferLen,
                    EncodingType encoding) {

  if (!cam) {
    throw Exception(ERR_ARG_NULL,
                    "Argument 'cam' may not be null for encoding.");
  }

  std::vector<uint8_t> dynamicBuffer;
  asn_enc_rval_t retVal;

  switch (encoding) {
  case XER_BASIC:
    retVal = xer_encode(&asn_DEF_CAM, (void *)cam, XER_F_BASIC, writeCallback,
                        (void *)&dynamicBuffer);
    break;
  case XER_CANONICAL:
    retVal = xer_encode(&asn_DEF_CAM, (void *)cam, XER_F_CANONICAL,
                        writeCallback, (void *)&dynamicBuffer);
    break;
  case DER_BER:
    retVal = der_encode(&asn_DEF_CAM, (void *)cam, writeCallback,
                        (void *)&dynamicBuffer);
    break;
  default:
    throw Exception(ERR_INVALID_ARG, "Invalid encoding type.");
  }

  if (retVal.encoded == -1) {
    std::stringstream errMsgStream;
    std::string xmlTagHighFrequencyContainerString = "HighFrequencyContainer";
    std::string xmlTag(retVal.failed_type->xml_tag);
    if (xmlTag.compare(xmlTagHighFrequencyContainerString) == 0) {
      errMsgStream << "No HighFrequencyContainer specified for CAM message "
                      "from station '"
                   << cam->header.stationID << "'. "
                   << "Add a BasicVehicleContainerHighFrequency or a "
                      "RSUContainerHighFrequency block to this message."
                   << std::endl;
    } else {
      errMsgStream << "CAM Encoding failed (Code=" << retVal.failed_type->name
                   << "). "
                   << "This is probably due to an invalid value of property '"
                   << retVal.failed_type->xml_tag
                   << "' in the message of Station '" << cam->header.stationID
                   << "'." << std::endl;
    }
    throw Exception(ERR_ENCODE, errMsgStream.str());
  }

  int requiredBufferSize = (int)dynamicBuffer.size();
  if (bufferLen < requiredBufferSize) {
    std::stringstream errMsgStream;
    errMsgStream
        << "The provided buffer is to small for the encoded message (Buffer "
           "size = '"
        << bufferLen << "', Message size = '" << requiredBufferSize << "')."
        << std::endl;
    throw Exception(ERR_BUFFER_OVERFLOW, errMsgStream.str());
  }

  std::copy(dynamicBuffer.begin(), dynamicBuffer.end(), buffer);
  return requiredBufferSize;
}

CAM_t *Encoder::decode(const uint8_t *buffer, int bufferLen,
                       EncodingType encoding) {

  if (!buffer) {
    throw Exception(ERR_ARG_NULL,
                    "Argument 'buffer' may not be null for decoding.");
  }

  if (bufferLen == 0) {
    throw Exception(ERR_ARG_NULL,
                    "Argument 'buffer' can not be empty for decoding.");
  }

  CAM_t *cam = nullptr;
  asn_dec_rval_t retVal;
  asn_codec_ctx_t opt_codec_ctx{};
  opt_codec_ctx.max_stack_size = 0;

  switch (encoding) {
  case XER_BASIC:
    retVal = xer_decode(&opt_codec_ctx, &asn_DEF_CAM, (void **)&cam, buffer,
                        bufferLen);
    break;
  case XER_CANONICAL:
    retVal = xer_decode(&opt_codec_ctx, &asn_DEF_CAM, (void **)&cam, buffer,
                        bufferLen);
    break;
  case DER_BER:
    retVal = ber_decode(&opt_codec_ctx, &asn_DEF_CAM, (void **)&cam, buffer,
                        bufferLen);
    break;
  default:
    throw Exception(ERR_INVALID_ARG, "Invalid encoding type.");
  }

  if (retVal.code != asn_dec_rval_code_e::RC_OK) {
    ASN_STRUCT_FREE(asn_DEF_CAM, cam);
    throw Exception(ERR_DECODE, "Decoding of CAM failed.");
  }

  return cam;
}

} 
} // namespace level::cam