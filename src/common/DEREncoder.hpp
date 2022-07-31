/**
 * @file DEREncoder.hpp
 * @author Klaus Weinbauer
 * @brief ASN1 based DER BER encoder.
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
#include <string>

#define DER_BER_ENCODING_TYPE_NAME "DER/BER"

namespace level {

/**
 * @brief ASN1 based DER BER encoder.
 *
 * @tparam T ASN1 type struct.
 */
template <typename T> class DEREncoder : public IEncoder<T> {

private:
  asn_TYPE_descriptor_t _typeDescriptor;

  static int writeCallback(const void *src, size_t size, void *container) {
    auto buffer = (std::vector<BYTE> *)container;

    for (size_t i = 0; i < size; i++) {
      BYTE data = ((BYTE *)src)[i];
      buffer->push_back(data);
    }

    return 0;
  }

public:
  DEREncoder(asn_TYPE_descriptor_t typeDescriptor)
      : _typeDescriptor(typeDescriptor) {}

  virtual ~DEREncoder() {}

  virtual std::vector<BYTE> encode(const T *message) {
    if (!message) {
      throw Exception(ERR_ARG_NULL, "Argument 'message' is null.");
    }

    std::vector<BYTE> buffer;
    asn_enc_rval_t retVal;
    retVal = der_encode(&_typeDescriptor, (void *)message, writeCallback,
                        (void *)&buffer);

    if (retVal.encoded == -1) {
      throw EncodeException(DER_BER_ENCODING_TYPE_NAME, TypeName<T>::get(),
                            retVal.failed_type->xml_tag,
                            retVal.failed_type->name);
    }

    return buffer;
  }

  virtual T *decode(const std::vector<BYTE> buffer) {
    auto array = std::make_unique<BYTE[]>(buffer.size());
    std::copy(buffer.begin(), buffer.end(), array.get());
    auto msg = decode(array.get(), buffer.size());
    return msg;
  }

  virtual T *decode(const BYTE *buffer, int bufferLen) {
    T *msg = nullptr;
    asn_dec_rval_t retVal;
    asn_codec_ctx_t opt_codec_ctx{};
    opt_codec_ctx.max_stack_size = 0;

    retVal = ber_decode(&opt_codec_ctx, &_typeDescriptor, (void **)&msg, buffer,
                        bufferLen);

    if (retVal.code != asn_dec_rval_code_e::RC_OK) {
      ASN_STRUCT_FREE(_typeDescriptor, msg);
      throw EncodeException(DER_BER_ENCODING_TYPE_NAME, TypeName<T>::get());
    }

    return msg;
  }
};

} // namespace level