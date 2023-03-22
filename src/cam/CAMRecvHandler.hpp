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
 * @file CAMRecvHandler.hpp
 * @author Klaus Weinbauer
 * @brief Receive handler for CAMs.
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <CAM.h>
#include <RecvHandler.hpp>

namespace level::cam {

/**
 * @brief Receive handler for CAMs.
 *
 */
class CAMRecvHandler : public RecvHandler<CAM> {
protected:
  virtual void freeMessage(CAM *msg) override {
    ASN_STRUCT_FREE(asn_DEF_CAM, msg);
  }
};

} // namespace level::cam