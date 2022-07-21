#include <CAMFactory.hpp>
#include <NetworkException.hpp>
#include <UDPNetworkAccessLayer.hpp>
#include <UDPSocket.hpp>

namespace level {
namespace cam {

UDPNetworkAccessLayer::UDPNetworkAccessLayer()
    : UDPNetworkAccessLayer(UDP_DEFAULT_PORT) {}

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

  try {
    int len = Factory::encoder().encode(cam, buffer, bufferSize, _encoding);
    _transSocket.sendTo(_port, (char *)buffer, len);
  } catch (const Exception &e) {
    throw e;
  } catch (const std::exception &e) {
    std::stringstream errMsg;
    errMsg << "Sending CAM failed (" << e.what() << ")." << std::endl;
    throw NetworkException(ERR, errMsg.str().c_str());
  }

  free(buffer);
}

void UDPNetworkAccessLayer::recvPacket(const char *buffer, int len) {
  CAM_t *cam = Factory::encoder().decode((uint8_t *)buffer, len, _encoding);
  recvCallback(cam);
}

void UDPNetworkAccessLayer::recvFailed(const Exception &ex) {
  recvFailedCallback(ex);
}

void UDPNetworkAccessLayer::setEncoding(EncodingType encoding) {
  _encoding = encoding;
}

} // namespace cam
} // namespace level
