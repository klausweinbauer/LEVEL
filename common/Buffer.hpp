#ifndef __Buffer_H
#define __Buffer_H

#define DEFAULT_BUFFER_SIZE 1024

#include <stdlib.h>

class Buffer
{
public:
    virtual size_t size() = 0;
    virtual size_t write(const void* src, size_t size,
        void* application_specific_key) = 0;
    virtual size_t copy(void* buffer, size_t size) = 0;
    virtual void reset() = 0;
};

#endif /* __Buffer_H */