/**
 * @file XEREncoder.hpp
 * @author Klaus Weinbauer
 * @brief ASN1 based XER encoder.
 * @version 0.1
 * @date 2022-07-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <EncodeException.hpp>
#include <IEncoder.hpp>
#include <Types.hpp>
#include <constr_TYPE.h>
#include <cstddef>
#include <string>
#include <vector>

#define XER_ENCODING_TYPE_NAME "XML"

namespace level {

/**
 * @brief ASN1 based XER encoder.
 *
 * @tparam T ASN1 type struct.
 */
template <typename T> class XEREncoder : public IEncoder<T> {

private:
  asn_TYPE_descriptor_t _typeDescriptor;
  xer_encoder_flags_e _encoding;

  static int writeCallback(const void *src, size_t size, void *container) {
    auto buffer = (std::vector<char> *)container;

    for (size_t i = 0; i < size; i++) {
      char data = ((char *)src)[i];
      buffer->push_back(data);
    }

    return 0;
  }

public:
  XEREncoder(asn_TYPE_descriptor_t typeDescriptor, xer_encoder_flags_e encoding)
      : _typeDescriptor(typeDescriptor), _encoding(encoding) {}

  virtual ~XEREncoder() {}

  virtual std::vector<char> encode(const T *message) {

    std::vector<char> buffer;
    asn_enc_rval_t retVal;
    retVal = xer_encode(&_typeDescriptor, (void *)message, _encoding,
                        writeCallback, (void *)&buffer);

    if (retVal.encoded == -1) {
      throw EncodeException(XER_ENCODING_TYPE_NAME, TypeName<T>::get(),
                            retVal.failed_type->xml_tag,
                            retVal.failed_type->name);
    }

    return buffer;
  }

  virtual T *decode(const std::vector<char> buffer) {
    auto array = std::make_unique<char[]>(buffer.size());
    std::copy(buffer.begin(), buffer.end(), array.get());
    auto msg = decode(array.get(), buffer.size());
    return msg;
  }

  virtual T *decode(const char *buffer, int bufferLen) {
    T *msg = nullptr;
    asn_dec_rval_t retVal;
    asn_codec_ctx_t opt_codec_ctx{};
    opt_codec_ctx.max_stack_size = 0;

    retVal = xer_decode(&opt_codec_ctx, &_typeDescriptor, (void **)&msg, buffer,
                        bufferLen);

    if (retVal.code != asn_dec_rval_code_e::RC_OK) {
      ASN_STRUCT_FREE(_typeDescriptor, msg);
      throw EncodeException(XER_ENCODING_TYPE_NAME, TypeName<T>::get());
    }

    return msg;
  }
};

} // namespace level