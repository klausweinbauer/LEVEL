#ifndef __UDPSocket_H
#define __UDPSocket_H

#include <string>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

#ifdef _WIN32
class WSASession
{
public:
    WSASession();
    ~WSASession();
private:
    WSAData data_;
};
#endif

class UDPSocket
{
private:
    bool recv_fail_;
    #ifdef _WIN32
    SOCKET sock_;
    #else
    int sock_;
    #endif

public:
    UDPSocket();
    ~UDPSocket();

    void sendTo(unsigned short port, const char* buffer, int len, int flags = 0);
    //void SendTo(sockaddr_in& address, const char* buffer, int len, int flags = 0);
    int recvFrom(char* buffer, int len, sockaddr_in* from_addr, int flags = 0);
    void bindSocket(unsigned short port);
    void close();
};

#endif // __UDPSocket_H