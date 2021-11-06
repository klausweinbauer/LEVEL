#pragma once

#include <PacketReceiver.hpp>

namespace c2x {
    class CAMReceiver : public PacketReceiver
    {
    protected:
        CAMReceiver();

        void decodeMessage(char* buffer, int len);

    public:
        static CAMReceiver& getInstance()
        {
            static CAMReceiver instance;
            return instance;
        }
    };
};