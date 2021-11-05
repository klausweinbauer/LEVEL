#include <c2xcommon.h>

#ifdef __cplusplus
namespace c2x {
#endif

void getErrMsg(int err, char* buffer, int size) {
    switch (err)
    {
    case ERR_MSG_NOT_FOUND:
        snprintf(buffer, size, "Message was not found.");
        break;
    case ERR_ALLOC_FAILED:
        snprintf(buffer, size, "Memory allocation failed.");
        break;
    case ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE:
        snprintf(buffer, size, "The HighFrequencyContainer of this CAM "
            "message has to be of type " 
            "HighFrequencyContainerBasicVehicleContainer");
        break;
    case ERR_NULL:
        snprintf(buffer, size, "Null pointer.");
        break;
    default:
        break;
    }
}

#pragma endregion

#ifdef __cplusplus
};
#endif