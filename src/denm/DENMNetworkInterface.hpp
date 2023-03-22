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
 * @file DENMNetworkInterface.hpp
 * @author Klaus Weinbauer
 * @brief Network interface to send and receive DENMs.
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <DENM.h>
#include <SocketNAL.hpp>

namespace level::denm {

/**
 * @brief Network interface to send and receive DENMs.
 *
 */
class DENMNetworkInterface : public SocketNAL<DENM> {
public:
  DENMNetworkInterface(std::shared_ptr<ISocket> sendSocket,
                       std::shared_ptr<ISocket> recvSocket,
                       std::shared_ptr<IEncoder<DENM>> encoder,
                       std::shared_ptr<IRecvHandler<DENM>> recvHandler,
                       std::shared_ptr<IErrorHandler> errorHandler)
      : SocketNAL(sendSocket, recvSocket, encoder, recvHandler, errorHandler) {}

  virtual ~DENMNetworkInterface() {}
};

} // namespace level::denm