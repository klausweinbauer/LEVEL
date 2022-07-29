/**
 * @file CAMEncoder.hpp
 * @author Klaus Weinbauer
 * @brief Encoders for CAMs.
 * @version 0.1
 * @date 2022-07-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <CAM.h>
#include <DEREncoder.hpp>
#include <XEREncoder.hpp>

namespace level::cam {

class DEREncoder : public level::DEREncoder<CAM> {
public:
  DEREncoder() : level::DEREncoder<CAM>(asn_DEF_CAM) {}
};

class XEREncoder : public level::XEREncoder<CAM> {
public:
  XEREncoder(xer_encoder_flags_e encoding)
      : level::XEREncoder<CAM>(asn_DEF_CAM, encoding) {}
};

} // namespace level::cam