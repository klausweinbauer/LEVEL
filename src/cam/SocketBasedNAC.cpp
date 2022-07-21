#include <IEncoder.hpp>
#include <ISocket.hpp>
#include <NetworkException.hpp>
#include <SocketBasedNAC.hpp>

namespace level {
namespace cam {

SocketBasedNAC::SocketBasedNAC(std::shared_ptr<ISocket> sendSocket,
                               std::shared_ptr<ISocket> recvSocket,
                               std::shared_ptr<IEncoder> encoder)
    : _sendSocket(sendSocket), _receiver(recvSocket), _encoder(encoder) {
  _receiver.recvPacketCallback = [this](const char *buffer, int len) {
    recvPacket(buffer, len);
  };
  _receiver.errCallback = [this](const Exception &ex) { recvFailed(ex); };
}

SocketBasedNAC::~SocketBasedNAC() {}

void SocketBasedNAC::send(const CAM_t *cam) {

  int bufferSize = 65535;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferSize);

  int len = _encoder->encode(cam, buffer, bufferSize);
  _sendSocket->sendTo((char *)buffer, len);

  free(buffer);
}

void SocketBasedNAC::recvPacket(const char *buffer, int len) {
  CAM_t *cam = _encoder->decode((uint8_t *)buffer, len);
  recvCallback(cam);
}

void SocketBasedNAC::recvFailed(const Exception &ex) { recvFailedCallback(ex); }

} // namespace cam
} // namespace level
