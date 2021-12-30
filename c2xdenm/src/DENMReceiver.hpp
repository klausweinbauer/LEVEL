#pragma once

#include <PacketReceiver.hpp>
#include <mutex>
#include <c2xcommon.h>

namespace c2x {

class DENMReceiver : public PacketReceiver
{
    protected:
        EncodingType encoding_;

        DENMReceiver();

        void decodeMessage(char* buffer, int len);

    public:
        void (*recvCallback)(int stationId, int sequenceNumber);

        void setEncoding(EncodingType encoding);
        EncodingType getEncoding();

        static DENMReceiver& getInstance()
        {
            static DENMReceiver instance;
            return instance;
        }
    };

};