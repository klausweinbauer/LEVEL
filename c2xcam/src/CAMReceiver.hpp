#pragma once

#include <PacketReceiver.hpp>

namespace c2x {
    class CAMReceiver : public PacketReceiver
    {
    protected:
        int last_error_ = 0;

        CAMReceiver();

        void decodeMessage(char* buffer, int len);

    public:
        void (*recvCallback)(int stationId);
        int getLastError();

        static CAMReceiver& getInstance()
        {
            static CAMReceiver instance;
            return instance;
        }
    };
};