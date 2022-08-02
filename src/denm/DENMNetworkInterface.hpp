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
  DENMNetworkInterface(
      std::shared_ptr<ISocket> sendSocket, std::shared_ptr<ISocket> recvSocket,
      std::shared_ptr<IEncoder<DENM>> encoder,
      std::function<void(DENM *)> recvCallback = nullptr,
      std::function<void(const Exception &)> errorCallback = nullptr)
      : SocketNAL(sendSocket, recvSocket, encoder, recvCallback,
                  errorCallback) {}

  virtual ~DENMNetworkInterface() {}
};

} // namespace level::denm