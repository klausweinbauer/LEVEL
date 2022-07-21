#pragma once

#include <INetworkInterface.hpp>
#include <ISocket.hpp>
#include <PacketReceiver.hpp>
#include <mutex>

namespace level {
namespace cam {

class SocketBasedNI : public INetworkInterface {
private:
  std::shared_ptr<ISocket> _sendSocket;
  PacketReceiver _receiver;
  std::shared_ptr<IEncoder> _encoder;
  std::mutex _transSocketLock;

  void recvPacket(const char *buffer, int len);
  void recvFailed(const Exception &ex);

public:
  SocketBasedNI(
      std::shared_ptr<ISocket> sendSocket, std::shared_ptr<ISocket> recvSocket,
      std::shared_ptr<IEncoder> encoder,
      std::function<void(CAM_t *)> recvCallback = nullptr,
      std::function<void(const Exception &)> recvFailedCallback = nullptr);
  virtual ~SocketBasedNI();

  SocketBasedNI(const SocketBasedNI &other) = delete;
  SocketBasedNI &operator=(const SocketBasedNI &other) = delete;

  virtual void send(const CAM_t *cam) override;
};

} // namespace cam
} // namespace level
