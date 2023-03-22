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