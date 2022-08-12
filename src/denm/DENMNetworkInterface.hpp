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