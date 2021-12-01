#pragma once

#include <stdint.h>

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
#define ERR_BUFFER_OVERFLOW -4
#define ERR_ENCODE -5
#define ERR_DECODE -6
#define ERR_RECEIVER_START -7
#define ERR_TRANSMITTER_START -8
#define ERR_ARG_NULL -9
#define ERR_INDEX_OUT_OF_RANGE -10
#define ERR_HIGH_FREQ_CONTAINER_TYPE -20
#define ERR_LOW_FREQ_CONTAINER_TYPE -21
#define ERR_CAM_ALREADY_EXISTS -22
#define ERR_SPECIAL_VEHICLE_CONTAINER_TYPE -23
#define ERR_DENM_ALREADY_EXISTS -40

void SHARED_EXPORT getErrMsg(int err, char* buffer, int size);
void SHARED_EXPORT getLastErrMsg(char* buffer, int size, int* actualSize);
void SHARED_EXPORT setLastErrMsg(const char* buffer, int size);

#ifdef __cplusplus
}
};
#endif