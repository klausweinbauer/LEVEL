#include <NetworkException.hpp>
#include <UDPSocket.hpp>
#include <memory>
#include <sstream>
#include <string.h>

namespace level {

UDPSocket::UDPSocket(unsigned short port, std::shared_ptr<ISyscall> syscall)
    : _port(port), _sys(syscall), _fd(0) {}

UDPSocket::~UDPSocket() {}

void UDPSocket::send(const char *buffer, int len) {}

int UDPSocket::recv(char *buffer, int len) { return 0; }

void UDPSocket::close() {}

void UDPSocket::bindSocket() {}

}; // namespace level