#pragma once

#include <PacketReceiver.hpp>
#include <c2xcommon.h>

namespace c2x {
    class CAMReceiver : public PacketReceiver
    {
    protected:
        int last_error_ = 0;
        EncodingType encoding_;

        CAMReceiver();

        void decodeMessage(char* buffer, int len);

    public:
        void (*recvCallback)(int stationId);
        int getLastError();
        EncodingType getEncoding();
        void setEncoding(EncodingType encoding);

        static CAMReceiver& getInstance()
        {
            static CAMReceiver instance;
            return instance;
        }
    };
};