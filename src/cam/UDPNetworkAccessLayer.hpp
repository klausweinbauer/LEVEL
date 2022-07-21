#pragma once

#include <INetworkInterface.hpp>
#include <PacketReceiver.hpp>
#include <UDPSocket.hpp>
#include <mutex>

#define UDP_DEFAULT_PORT 5899

namespace level {
namespace cam {

class UDPNetworkAccessLayer : public INetworkInterface {
private:
  const unsigned short _port;
  UDPSocket _transSocket;
  std::mutex _transSocketLock;
  PacketReceiver _receiver;
  EncodingType _encoding;

  void recvPacket(const char *buffer, int len);
  void recvFailed(const Exception &ex);

public:
  UDPNetworkAccessLayer();
  UDPNetworkAccessLayer(unsigned short port,
                        EncodingType encoding = EncodingType::DER_BER);
  virtual ~UDPNetworkAccessLayer();

  UDPNetworkAccessLayer(const UDPNetworkAccessLayer &other) = delete;
  UDPNetworkAccessLayer &operator=(const UDPNetworkAccessLayer &other) = delete;

  virtual void send(const CAM_t *cam) override;
  virtual void setEncoding(EncodingType encoding) override;
};

} // namespace cam
} // namespace level
