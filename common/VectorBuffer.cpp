#include <VectorBuffer.hpp>
#include <stdlib.h>

VectorBuffer::VectorBuffer()
{

}

VectorBuffer::VectorBuffer(int initial_size) : buffer_(initial_size)
{

}

size_t VectorBuffer::size()
{
    return buffer_.size();
}

size_t VectorBuffer::write(const void* src, size_t size, void* application_specific_key)
{
    for (size_t i = 0; i < size; i++)
    {
        char c = ((char*)src)[i];
        buffer_.push_back(c);
    }

    return size;
}

size_t VectorBuffer::copy(void* buffer, size_t size)
{
    size_t i = 0;
    while (i < size && i < buffer_.size())
    {
        ((char*)buffer)[i] = buffer_[i];
        i++;
    }

    return i;
}

void VectorBuffer::reset()
{
    buffer_.clear();
}