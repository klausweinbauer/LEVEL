#include <c2xcommon.h>
#include <cstdio>
#include <string.h>
#include <sstream>

#ifdef __cplusplus
namespace c2x {
#endif

static char* lastErrMsg = nullptr;
static int lastErrMsgSize = 0;

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
    case ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE:
        snprintf(buffer, size, "The HighFrequencyContainer of this CAM "
            "message has to be of type " 
            "HighFrequencyContainerBasicVehicleContainer");
        break;
    case ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE:
        snprintf(buffer, size, "The LowFrequencyContainer of this CAM "
            "message has to be of type " 
            "LowFrequencyContainerBasicVehicleContainer");
        break;
    case ERR_CAM_ALREADY_EXISTS:
        snprintf(buffer, size, "A CAM with this ID already exists.");
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
    if (!lastErrMsg) {
        return;
    }

    int cpySize = (std::min)(size, lastErrMsgSize);
    memcpy(buffer, lastErrMsg, cpySize);

    if (actualSize) {
        *actualSize = cpySize;
    }
}

void setLastErrMsg(const char* buffer, int size)
{
    if (!buffer) {
        return;
    }

    if (lastErrMsg) {
        lastErrMsg = (char*)realloc(lastErrMsg, size);
    }
    else {
        lastErrMsg = (char*)malloc(size);
    }

    if (!lastErrMsg) {
        return;
    }

    memcpy(lastErrMsg, buffer, size);
    lastErrMsgSize = size;
}

#pragma endregion

#ifdef __cplusplus
};
#endif