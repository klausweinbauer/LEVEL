#include <UDPSocket.hpp>
#include <string.h>
#include <iostream>

#ifdef _WIN32
#include <WS2tcpip.h>
#else
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>
#endif

namespace c2x {
#ifdef _WIN32
WSASession::WSASession()
{
    int ret = WSAStartup(MAKEWORD(2, 2), &data_);
    if (ret != 0)
        throw std::system_error(WSAGetLastError(), std::system_category(), "WSAStartup Failed");
}

WSASession::~WSASession()
{
    WSACleanup();
}
#endif

#ifdef _WIN32
UDPSocket::UDPSocket() : recv_fail_(false)
{
    sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_ == INVALID_SOCKET)
        throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket"); 
    int trueflag = 1;    
    int res = setsockopt(sock_, SOL_SOCKET, SO_BROADCAST, (char*)&trueflag, sizeof trueflag);
    if (res < 0)
        throw std::system_error(WSAGetLastError(), std::system_category(), "Error set broadcast options");
}
#else
UDPSocket::UDPSocket()
{
    sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_ < 0)
        throw std::invalid_argument(strerror(errno));

    int trueflag = 1;
    if (setsockopt(sock_, SOL_SOCKET, SO_BROADCAST,
        &trueflag, sizeof(trueflag)) < 0)
    {
        ::close(sock_);
        throw std::invalid_argument(strerror(errno));
    }
}
#endif

#ifdef _WIN32
UDPSocket::~UDPSocket()
{
    closesocket(sock_);
}
#else
UDPSocket::~UDPSocket()
{
    ::close(sock_);
}
#endif

void UDPSocket::sendTo(unsigned short port, const char* buffer, int len, int flags)
{
    sockaddr_in add{};
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr("255.255.255.255");
    add.sin_port = htons(port);

    #ifdef _WIN32
    int ret = sendto(sock_, buffer, len, flags, reinterpret_cast<SOCKADDR*>(&add), sizeof(add));
    if (ret < 0)
    {
        throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
    }
    #else
    int ret = sendto(sock_, buffer, len, flags, reinterpret_cast<sockaddr*>(&add), sizeof(add));
    if (ret < 0)
    {
        throw std::invalid_argument(strerror(errno));
    }
    #endif
}

int UDPSocket::recvFrom(char* buffer, int len, sockaddr_in* from_addr, int flags)
{
    recv_fail_ = false;
    int ret = recvfrom(sock_, buffer, len, flags, nullptr, nullptr);
    if (ret < 0)
    {
        if (!recv_fail_)
        {
#ifdef _WIN32
            throw std::system_error(WSAGetLastError(), std::system_category(), "recvfrom failed");
#else
            throw std::invalid_argument(strerror(errno));
#endif
        }        
    }
    else {
        // make the buffer zero terminated
        buffer[ret] = 0;
    }

    return ret;
}

void UDPSocket::close()
{
#ifdef WIN32
    recv_fail_ = true;
    shutdown(sock_, SD_BOTH);
#else
    shutdown(sock_, SHUT_RDWR);
#endif
}

void UDPSocket::bindSocket(unsigned short port)
{
    sockaddr_in add{};
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = htonl(INADDR_ANY);
    add.sin_port = htons(port);

    #ifdef _WIN32
    int ret = bind(sock_, reinterpret_cast<SOCKADDR*>(&add), sizeof(add));
    if (ret < 0)
    {        
        throw std::system_error(WSAGetLastError(), std::system_category(), "Bind failed");
    }
    #else
    int ret = bind(sock_, reinterpret_cast<sockaddr*>(&add), sizeof(add));
    if (ret < 0)
    {        
        throw std::invalid_argument(strerror(errno));
    }
    #endif
}
};