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
 * @file CAMNetworkInterface.hpp
 * @author Klaus Weinbauer
 * @brief Network interface to send and receive CAMs.
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <CAM.h>
#include <SocketNAL.hpp>

namespace level::cam {

/**
 * @brief Network interface to send and receive CAMs.
 *
 */
class CAMNetworkInterface : public SocketNAL<CAM> {
public:
  CAMNetworkInterface(std::shared_ptr<ISocket> sendSocket,
                      std::shared_ptr<ISocket> recvSocket,
                      std::shared_ptr<IEncoder<CAM>> encoder,
                      std::shared_ptr<IRecvHandler<CAM>> recvHandler,
                      std::shared_ptr<IErrorHandler> errorHandler)
      : SocketNAL(sendSocket, recvSocket, encoder, recvHandler, errorHandler) {}

  virtual ~CAMNetworkInterface() {}
};

} // namespace level::cam