#pragma once

#include <INetworkInterface.hpp>
#include <UDPSocket.hpp>
#include <mutex>

#define UDP_DEFAULT_PORT 5899

namespace c2x::cam {

class UDPNetworkAccessLayer : public INetworkInterface {
private:
  const unsigned short _port;
  UDPSocket _transSocket;
  std::mutex _transSocketLock;

public:
  UDPNetworkAccessLayer();
  UDPNetworkAccessLayer(unsigned short port);
  virtual ~UDPNetworkAccessLayer();

  UDPNetworkAccessLayer(const UDPNetworkAccessLayer &other) = delete;
  UDPNetworkAccessLayer &operator=(const UDPNetworkAccessLayer &other) = delete;

  virtual void send(const CAM_t *cam,
                    EncodingType encoding = EncodingType::DER_BER) override;
};

} // namespace c2x::cam
