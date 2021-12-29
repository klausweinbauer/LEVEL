#pragma once

#include <PacketReceiver.hpp>
#include <mutex>

namespace c2x {

class DENMReceiver : public PacketReceiver
{
    protected:
        DENMReceiver();

        void decodeMessage(char* buffer, int len);

    public:
        void (*recvCallback)(int stationId, int sequenceNumber);

        static DENMReceiver& getInstance()
        {
            static DENMReceiver instance;
            return instance;
        }
    };

};