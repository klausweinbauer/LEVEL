/**
 * @file level_common.h
 * @author Klaus Weinbauer
 * @brief Standardized components and functions for LEVEL.
 * @version 0.1
 * @date 2022-07-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <stdint.h>

#ifdef _WIN32
#ifdef SHARED_EXPORT
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT __declspec(dllimport)
#endif
#else
#define SHARED_EXPORT
#endif

#ifdef __cplusplus
namespace level {

extern "C" {
#endif

/**
 * @brief Collection of supported encoding standards for message
 * en- and decoding
 *
 */
enum EncodingType {
  XER_BASIC,
  /**<
    XML Encoding Rules according to ITU-T.
    Use this encoding to achieve better human readability.
  */
  XER_CANONICAL,
  /**<
    XML Encoding Rules according to ITU-T.
    Use this encoding to achieve better human readability.
  */
  DER_BER
  /**<
    Basic and Distiributed Encoding Rules according to ITU-T.
    Use this for a compact encoding and smaller sizes for packages sent over
    the network.
  */
};

/**
 * @brief This function provides a general description for an error code.
 *
 * @param err The \ref level_error.h "error code" for which the description
 * should be read.
 * @param buffer This is the buffer where the error message is to be copied.
 * @param size The size of the buffer.
 */
void SHARED_EXPORT getErrMsg(int err, char *buffer, int size);

/**
 * @brief Provides a description for the last error.
 *
 * @param buffer This is the buffer where the error message is to be copied.
 * @param size The size of the buffer.
 * @param actualSize Returns the actual length of the error message.
 */
void SHARED_EXPORT getLastErrMsg(char *buffer, int size, int *actualSize);

/**
 * @brief Function to set a detailed error message.
 *
 * @param buffer The buffer where the error message is stored.
 * @param size The length of the error message.
 */
void SHARED_EXPORT setLastErrMsg(const char *buffer, int size);

#ifdef __cplusplus
}
};
#endif