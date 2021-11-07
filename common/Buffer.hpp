#pragma once

#define DEFAULT_BUFFER_SIZE 1024

#include <stdlib.h>

namespace c2x {
    class Buffer
    {
    public:
        virtual size_t size() = 0;
        virtual size_t write(const void* src, size_t size,
            void* application_specific_key) = 0;
        virtual size_t copy(void* buffer, size_t size) = 0;
        virtual void reset() = 0;
    };
}