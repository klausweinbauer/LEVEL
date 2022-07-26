#include <IEncoder.hpp>
#include <ISocket.hpp>
#include <NetworkException.hpp>
#include <SocketBasedNI.hpp>

namespace level {
namespace cam {

SocketBasedNI::SocketBasedNI(
    std::shared_ptr<ISocket> sendSocket, std::shared_ptr<ISocket> recvSocket,
    std::shared_ptr<IEncoder> encoder,
    std::function<void(CAM_t *)> recvCallback,
    std::function<void(const Exception &)> recvFailedCallback)
    : _sendSocket(sendSocket), _receiver(recvSocket), _encoder(encoder) {

  this->recvCallback = recvCallback;
  this->recvFailedCallback = recvFailedCallback;
  _receiver.recvPacketCallback = [this](const char *buffer, int len) {
    recvPacket(buffer, len);
  };
  _receiver.errCallback = [this](const Exception &ex) { recvFailed(ex); };
  _receiver.start();
}

SocketBasedNI::~SocketBasedNI() {}

void SocketBasedNI::send(const CAM_t *cam) {

  int bufferSize = 65535;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferSize);

  int len = _encoder->encode(cam, buffer, bufferSize);
  _sendSocket->send((char *)buffer, len);

  free(buffer);
}

void SocketBasedNI::recvPacket(const char *buffer, int len) {
  CAM_t *cam = _encoder->decode((uint8_t *)buffer, len);
  recvCallback(cam);
}

void SocketBasedNI::recvFailed(const Exception &ex) { recvFailedCallback(ex); }

} // namespace cam
} // namespace level
