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