#pragma once

#include <stdint.h>
#include <cstdio>

#ifdef _WIN32
#ifdef shared_EXPORTS
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT __declspec(dllimport)
#endif
#else
#define SHARED_EXPORT
#endif

#ifdef __cplusplus
namespace c2x {

extern "C"
{
#endif

#define ERR_MSG_NOT_FOUND -1
#define ERR_ALLOC_FAILED -2
#define ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE -3
#define ERR_NULL -4

typedef struct Position {
    int latitude = 0;
    int longitude = 0;
    int confidenceMajor = 0;
    int confidenceMinor = 0;
    int confidenceMajorOrientation = 0;
    int altitudeValue = 0;
    int altitudeConfidence = 0;
} Position;

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

#ifdef __cplusplus
}
};
#endif