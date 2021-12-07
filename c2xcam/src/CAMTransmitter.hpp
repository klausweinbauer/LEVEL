#pragma once

#include <thread>
#include <mutex>

#define TRANSMIT_BUFFER_LEN 65535

namespace c2x {
    class CAMTransmitter
    {
    private:
        unsigned short port_ = 0;
        bool thread_running_ = false;
        unsigned int interval_ms_ = 500;
        std::thread send_thread_;
        int stationId_ = -1;
        int* transmit_ids_ = nullptr;
        int transmit_ids_size_ = 0;
        std::mutex transmit_ids_lock_;
        int last_error_ = 0;

        CAMTransmitter();

        static void send();

    public:
        static CAMTransmitter& getInstance()
        {
            static CAMTransmitter instance;
            return instance;
        }

        CAMTransmitter(CAMTransmitter const&) = delete;
        void operator=(CAMTransmitter const&) = delete;

        void start(unsigned short port);
        void stop();
        void setInterval(unsigned int interval_ms);
        int setIDsToTransmit(int *ids, int size);
        unsigned int getInterval();
        int getLastError();
    };
};