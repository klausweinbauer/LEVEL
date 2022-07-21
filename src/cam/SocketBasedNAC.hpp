#pragma once

#include <INetworkInterface.hpp>
#include <ISocket.hpp>
#include <PacketReceiver.hpp>
#include <mutex>

namespace level {
namespace cam {

class SocketBasedNAC : public INetworkInterface {
private:
  std::shared_ptr<ISocket> _sendSocket;
  PacketReceiver _receiver;
  std::shared_ptr<IEncoder> _encoder;
  std::mutex _transSocketLock;

  void recvPacket(const char *buffer, int len);
  void recvFailed(const Exception &ex);

public:
  SocketBasedNAC(std::shared_ptr<ISocket> sendSocket,
                 std::shared_ptr<ISocket> recvSocket,
                 std::shared_ptr<IEncoder> encoder);
  virtual ~SocketBasedNAC();

  SocketBasedNAC(const SocketBasedNAC &other) = delete;
  SocketBasedNAC &operator=(const SocketBasedNAC &other) = delete;

  virtual void send(const CAM_t *cam) override;
};

} // namespace cam
} // namespace level
