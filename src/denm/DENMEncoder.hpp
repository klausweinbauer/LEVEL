/**
 * @file DENMEncoder.hpp
 * @author Klaus Weinbauer
 * @brief Encoders for DENMs.
 * @version 0.1
 * @date 2022-07-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <DENM.h>
#include <DEREncoder.hpp>
#include <XEREncoder.hpp>

namespace level::denm {

class DENMDEREncoder : public level::DEREncoder<DENM> {
public:
  DENMDEREncoder() : level::DEREncoder<DENM>(asn_DEF_DENM) {}
};

class DENMXEREncoder : public level::XEREncoder<DENM> {
public:
  DENMXEREncoder(xer_encoder_flags_e encoding)
      : level::XEREncoder<DENM>(asn_DEF_DENM, encoding) {}
};

} // namespace level::denm