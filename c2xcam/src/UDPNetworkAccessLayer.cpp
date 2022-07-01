#include <Factory.hpp>
#include <NetworkException.hpp>
#include <UDPNetworkAccessLayer.hpp>
#include <UDPSocket.hpp>

namespace c2x::cam {

UDPNetworkAccessLayer::UDPNetworkAccessLayer()
    : UDPNetworkAccessLayer(UDP_DEFAULT_PORT) {}

UDPNetworkAccessLayer::UDPNetworkAccessLayer(unsigned short port)
    : _port(port) {}

UDPNetworkAccessLayer::~UDPNetworkAccessLayer() {}

void UDPNetworkAccessLayer::send(const CAM_t *cam, EncodingType encoding) {

#ifdef WIN32
  WSASession session;
#endif

  int bufferSize = 65535;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferSize);

  try {
    int len = Factory::encoder().encode(cam, buffer, bufferSize, encoding);
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

} // namespace c2x::cam
