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