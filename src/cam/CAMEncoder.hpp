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

class CAMDEREncoder : public level::DEREncoder<CAM> {
public:
  CAMDEREncoder() : level::DEREncoder<CAM>(asn_DEF_CAM) {}
};

class CAMXEREncoder : public level::XEREncoder<CAM> {
public:
  CAMXEREncoder(xer_encoder_flags_e encoding)
      : level::XEREncoder<CAM>(asn_DEF_CAM, encoding) {}
};

} // namespace level::cam