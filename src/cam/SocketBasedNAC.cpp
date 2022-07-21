#include <CAMFactory.hpp>
#include <NetworkException.hpp>
#include <SocketBasedNAC.hpp>
#include <UDPSocket.hpp>

namespace level {
namespace cam {

SocketBasedNAC::SocketBasedNAC(std::shared_ptr<ISocket> sendSocket,
                               std::shared_ptr<ISocket> recvSocket,
                               EncodingType encoding)
    : _sendSocket(sendSocket), _receiver(recvSocket), _encoding(encoding) {
  _receiver.recvPacketCallback = [this](const char *buffer, int len) {
    recvPacket(buffer, len);
  };
  _receiver.errCallback = [this](const Exception &ex) { recvFailed(ex); };
}

SocketBasedNAC::~SocketBasedNAC() {}

void SocketBasedNAC::send(const CAM_t *cam) {

  int bufferSize = 65535;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferSize);

  int len = Factory::encoder().encode(cam, buffer, bufferSize, _encoding);
  _sendSocket->sendTo((char *)buffer, len);

  free(buffer);
}

void SocketBasedNAC::recvPacket(const char *buffer, int len) {
  CAM_t *cam = Factory::encoder().decode((uint8_t *)buffer, len, _encoding);
  recvCallback(cam);
}

void SocketBasedNAC::recvFailed(const Exception &ex) { recvFailedCallback(ex); }

} // namespace cam
} // namespace level
