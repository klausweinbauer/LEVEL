/**
 * @file level_error.h
 * @author Klaus Weinbauer
 * @brief Error codes used in LEVEL.
 * @version 0.1
 * @date 2022-07-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#ifdef __cplusplus
namespace level {

extern "C" {
#endif

/**
 * @brief General error code
 *
 */
#define ERR -1

/**
 * @brief Memory allocation for new messages failed
 *
 */
#define ERR_ALLOC_FAILED -2

/**
 * @brief Null reference exception
 *
 */
#define ERR_NULL -3

/**
 * @brief The provided buffer is to small
 *
 */
#define ERR_BUFFER_OVERFLOW -4

/**
 * @brief Message encoding failed
 *
 */
#define ERR_ENCODE -5

/**
 * @brief Message decoding failed
 *
 */
#define ERR_DECODE -6

/**
 * @brief Starting the receiver failed
 *
 */
#define ERR_RECEIVER_START -7

/**
 * @brief Starting the transmitter failed
 *
 */
#define ERR_TRANSMITTER_START -8

/**
 * @brief Argument which shouldn't be null is null
 *
 */
#define ERR_ARG_NULL -9

/**
 * @brief Index is out of range
 *
 */
#define ERR_INDEX_OUT_OF_RANGE -10

/**
 * @brief Provided argument is invalid
 *
 */
#define ERR_INVALID_ARG -11

/**
 * @brief Message does not exist in database
 *
 */
#define ERR_MSG_NOT_FOUND -12

/**
 * @brief Service detected an invalid operation
 *
 */
#define ERR_INVALID_OPERATION -13

/**
 * @brief Invalid type for high frequency container
 *
 */
#define ERR_HIGH_FREQ_CONTAINER_TYPE -40

/**
 * @brief Invalid type for low frequency container
 *
 */
#define ERR_LOW_FREQ_CONTAINER_TYPE -41

/**
 * @brief CAM with ID already exists
 *
 */
#define ERR_CAM_ALREADY_EXISTS -42

/**
 * @brief Invalid type for special vehicle container
 *
 */
#define ERR_SPECIAL_VEHICLE_CONTAINER_TYPE -43

/**
 * @brief DENM with ID and Sequence Number already exists
 *
 */
#define ERR_DENM_ALREADY_EXISTS -60

#ifdef __cplusplus
}
};
#endif