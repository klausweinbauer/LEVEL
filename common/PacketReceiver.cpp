#include <PacketReceiver.hpp>
#include <iostream>
#include <Error.hpp>

#ifndef WIN32
#include <unistd.h>
#endif

namespace c2x {
    static UDPSocket* socket_;

    PacketReceiver::PacketReceiver()
    {

    }

    PacketReceiver::~PacketReceiver()
    {
        
    }

    void PacketReceiver::start(unsigned short port)
    {
        if (thread_running_)
        {
            return;
        }

        port_ = port;

        thread_running_ = true;

        receiving_thread_ = std::thread(receive, this);
    }

    void PacketReceiver::receive(void* arg)
    {
        PacketReceiver* receiver = ((PacketReceiver*)arg);

    #ifdef WIN32
        WSASession session;
    #endif
        socket_ = new UDPSocket();
        socket_->bindSocket(receiver->port_);

        char* buffer = (char*)malloc(RECEIVE_BUFFER_LEN);
        while (receiver->thread_running_)
        {
            try
            {
                sockaddr_in from_addr;
                int len = socket_->recvFrom(buffer, RECEIVE_BUFFER_LEN, &from_addr);
                if (len < 0)
                {
                    continue;
                }
                
                receiver->decodeMessage(buffer, len);
            }
            catch (const std::exception& ex)
            {
                std::cout << "[Error] PacketReceiver::receive() | " << ex.what() << std::endl;

                const int wait_time = 10000;
                #ifdef WIN32
                Sleep(wait_time);
                #else
                usleep(wait_time * 1000);
                #endif
            }
        }
        free(buffer);
    }

    void PacketReceiver::stop()
    {
        if (!thread_running_)
        {
            return;
        }

        thread_running_ = false;
        if (socket_ != nullptr) 
        {
            socket_->close();
            delete socket_;
            socket_ = nullptr;
        }
        receiving_thread_.join();
    }
};