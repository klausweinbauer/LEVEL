#pragma once 

#include <vector>
#include <Buffer.hpp>
#include <stdlib.h>

#define DEFAULT_BUFFER_SIZE 1024

namespace c2x 
{
    class VectorBuffer : public Buffer
    {
    private:
        std::vector<char> buffer_;

    public:
        VectorBuffer();
        VectorBuffer(int initial_size);

        virtual size_t size();
        virtual size_t write(const void* src, size_t size,
            void* application_specific_key);
        virtual size_t copy(void* buffer, size_t size);
        virtual void reset();
    };
};