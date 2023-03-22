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
 * @file DENMRecvHandler.hpp
 * @author Klaus Weinbauer
 * @brief Receive handler for DENMs.
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <DENM.h>
#include <RecvHandler.hpp>

namespace level::denm {

/**
 * @brief Receive handler for DENMs.
 *
 */
class DENMRecvHandler : public RecvHandler<DENM> {
protected:
  virtual void freeMessage(DENM *msg) override {
    ASN_STRUCT_FREE(asn_DEF_DENM, msg);
  }
};

} // namespace level::denm