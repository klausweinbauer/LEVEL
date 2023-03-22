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
    auto buffer = (std::vector<char> *)container;

    for (size_t i = 0; i < size; i++) {
      char data = ((char *)src)[i];
      buffer->push_back(data);
    }

    return 0;
  }

public:
  DEREncoder(asn_TYPE_descriptor_t typeDescriptor)
      : _typeDescriptor(typeDescriptor) {}

  virtual ~DEREncoder() {}

  virtual std::vector<char> encode(const T *message) {
    if (!message) {
      throw Exception(ERR_ARG_NULL, "Argument 'message' is null.");
    }

    std::vector<char> buffer;
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