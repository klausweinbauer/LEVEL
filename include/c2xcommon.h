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
#define ERR_NULL -3
#define ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE -4
#define ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE -5

typedef struct Position {
    int latitude = 0;
    int longitude = 0;
    int confidenceMajor = 0;
    int confidenceMinor = 0;
    int confidenceMajorOrientation = 0;
    int altitudeValue = 0;
    int altitudeConfidence = 0;
} Position;

typedef struct DeltaPathPoint {
    int deltaLatitude = 0;
    int deltaLongitude = 0;
    int deltaAltitude = 0;
    int deltaTime = 0;
} DeltaPathPoint;

void getErrMsg(int err, char* buffer, int size);

#ifdef __cplusplus
}
};
#endif