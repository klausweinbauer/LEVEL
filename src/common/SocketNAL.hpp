/**
 * @file SocketNAL.hpp
 * @author Klaus Weinbauer
 * @brief Socket based network access layer implementation.
 * @version 0.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <IEncoder.hpp>
#include <INetworkInterface.hpp>
#include <ISocket.hpp>
#include <memory>
#include <thread>

#define SOCKET_NAL_BUFFER_SIZE_MAX 65535
#define SOCKET_NAL_TIMEOUT 100

namespace level {

/**
 * @brief Socket based network access layer implementation.
 *
 * @tparam T Message type.
 */
template <typename T> class SocketNAL : public INetworkInterface<T> {
private:
  std::shared_ptr<ISocket> _sendSocket;
  std::shared_ptr<ISocket> _recvSocket;
  std::shared_ptr<IEncoder<T>> _encoder;
  std::shared_ptr<IRecvHandler<T>> _recvHandler;
  std::shared_ptr<IErrorHandler> _errHandler;
  bool _recvActive;
  std::thread _recvThread;
  char *_buffer;
  int _bufferSize;

  void receive() {
    while (_recvActive) {
      T *decodedMsg;

      try {
        int recvSize =
            _recvSocket->recv(_buffer, _bufferSize, SOCKET_NAL_TIMEOUT);
        if (recvSize == 0) {
          continue;
        }
        decodedMsg = _encoder->decode(_buffer, recvSize);
      } catch (const Exception &e) {

        try {
          _errHandler->invoke(e);
        } catch (const std::exception &) {
        }

        continue;
      }

      try {
        _recvHandler->invoke(decodedMsg);
      } catch (const std::exception &) {
      }
    }
  }

public:
  virtual ~SocketNAL() {
    _recvActive = false;
    _recvThread.join();
    free(_buffer);
  }

  SocketNAL(std::shared_ptr<ISocket> sendSocket,
            std::shared_ptr<ISocket> recvSocket,
            std::shared_ptr<IEncoder<T>> encoder,
            std::shared_ptr<IRecvHandler<T>> recvHandler,
            std::shared_ptr<IErrorHandler> errorHandler)
      : _sendSocket(sendSocket), _recvSocket(recvSocket), _encoder(encoder),
        _recvHandler(recvHandler), _errHandler(errorHandler),
        _recvActive(false),
        _buffer((char *)calloc(1, sizeof(char) * SOCKET_NAL_BUFFER_SIZE_MAX)),
        _bufferSize(SOCKET_NAL_BUFFER_SIZE_MAX) {

    if (!_sendSocket) {
      free(_buffer);
      throw Exception(ERR_ARG_NULL, "Argument 'sendSocket' is null.");
    }
    if (!_recvSocket) {
      free(_buffer);
      throw Exception(ERR_ARG_NULL, "Argument 'recvSocket' is null.");
    }
    if (!_encoder) {
      free(_buffer);
      throw Exception(ERR_ARG_NULL, "Argument 'encoder' is null.");
    }

    _recvActive = true;
    _recvThread = std::thread(&SocketNAL::receive, this);
  }

  virtual bool send(const T *msg) {
    std::vector<char> encodedMsg = _encoder->encode(msg);
    auto array = std::make_unique<char[]>(encodedMsg.size());
    std::copy(encodedMsg.begin(), encodedMsg.end(), array.get());
    return _sendSocket->send(array.get(), encodedMsg.size());
  }
};

} // namespace level