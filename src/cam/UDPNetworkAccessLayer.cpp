#include <CAMFactory.hpp>
#include <NetworkException.hpp>
#include <UDPNetworkAccessLayer.hpp>
#include <UDPSocket.hpp>

namespace level {
namespace cam {

UDPNetworkAccessLayer::UDPNetworkAccessLayer(unsigned short port,
                                             EncodingType encoding)
    : _port(port), _receiver(port), _encoding(encoding) {
  _receiver.recvPacketCallback = [this](const char *buffer, int len) {
    recvPacket(buffer, len);
  };
  _receiver.errCallback = [this](const Exception &ex) { recvFailed(ex); };
}

UDPNetworkAccessLayer::~UDPNetworkAccessLayer() {}

void UDPNetworkAccessLayer::send(const CAM_t *cam) {

  int bufferSize = 65535;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferSize);

  int len = Factory::encoder().encode(cam, buffer, bufferSize, _encoding);
  _transSocket.sendTo(_port, (char *)buffer, len);

  free(buffer);
}

void UDPNetworkAccessLayer::recvPacket(const char *buffer, int len) {
  CAM_t *cam = Factory::encoder().decode((uint8_t *)buffer, len, _encoding);
  recvCallback(cam);
}

void UDPNetworkAccessLayer::recvFailed(const Exception &ex) {
  recvFailedCallback(ex);
}

} // namespace cam
} // namespace level
