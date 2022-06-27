#include <c2xcommon.h>
#include <cstdio>
#include <string.h>
#include <sstream>
#include <mutex>

#ifdef __cplusplus
namespace c2x {
#endif

static char* last_err_msg = nullptr;
static int last_err_msg_size = 0;
static std::mutex err_lock; 

void getErrMsg(int err, char* buffer, int size) {
    switch (err)
    {
    case ERR_MSG_NOT_FOUND:
        snprintf(buffer, size, "Message was not found.");
        break;
    case ERR_ALLOC_FAILED:
        snprintf(buffer, size, "Memory allocation failed.");
        break;
    case ERR_NULL:
        snprintf(buffer, size, "Null pointer.");
        break;
    case ERR_BUFFER_OVERFLOW:
        snprintf(buffer, size, "Buffer is too small.");
        break;
    case ERR_ENCODE:
        snprintf(buffer, size, "Encoding failed.");
        break;
    case ERR_DECODE:
        snprintf(buffer, size, "Decoding failed.");
        break;
    case ERR_RECEIVER_START:
        snprintf(buffer, size, "Starting the receiver failed.");
        break;
    case ERR_TRANSMITTER_START:
        snprintf(buffer, size, "Starting the transmitter failed.");
        break;
    case ERR_ARG_NULL:
        snprintf(buffer, size, "Argument was null.");
        break;
    case ERR_INDEX_OUT_OF_RANGE:
        snprintf(buffer, size, "Index is out of range.");
        break;
    case ERR_HIGH_FREQ_CONTAINER_TYPE:
        snprintf(buffer, size, "Wrong Type of HighFrequencyContainer.");
        break;
    case ERR_LOW_FREQ_CONTAINER_TYPE:
        snprintf(buffer, size, "Wrong Type of LowFrequencyContainer.");
        break;
    case ERR_CAM_ALREADY_EXISTS:
        snprintf(buffer, size, "A CAM with this ID already exists.");
        break;
    case ERR_SPECIAL_VEHICLE_CONTAINER_TYPE:
        snprintf(buffer, size, "Wrong Type of SpecialVehicleContainer.");
        break;
    case ERR_DENM_ALREADY_EXISTS:
        snprintf(buffer, size, "A DENM with this ID and SequenceNumber already exists.");
        break;
    default:
        break;
    }
}

void getLastErrMsg(char* buffer, int size, int* actualSize)
{
    if (actualSize) {
        *actualSize = 0;
    }
    err_lock.lock();
    if (!last_err_msg) {
        err_lock.unlock();
        return;
    }

    int cpySize = (std::min)(size, last_err_msg_size);
    strncpy(buffer, last_err_msg, cpySize);

    if (actualSize) {
        *actualSize = cpySize;
    }
    err_lock.unlock();
}

void setLastErrMsg(const char* buffer, int size)
{
    err_lock.lock();
    if (!buffer) {
        err_lock.unlock();
        return;
    }

    if (last_err_msg) {
        last_err_msg = (char*)realloc(last_err_msg, size);
    }
    else {
        last_err_msg = (char*)malloc(size);
    }

    if (!last_err_msg) {
        err_lock.unlock();
        return;
    }

    strncpy(last_err_msg, buffer, size);
    last_err_msg_size = size;
    err_lock.unlock();
}

#pragma endregion

#ifdef __cplusplus
};
#endif