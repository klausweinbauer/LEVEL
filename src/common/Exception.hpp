/**
 * @file Exception.hpp
 * @author Klaus Weinbauer
 * @brief Base exception class for all exceptions used in this library.
 * @version 0.1
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <exception>
#include <level_common.h>
#include <level_error.h>
#include <sstream>
#include <string>

namespace level {

/**
 * @brief Base exception class for all exceptions used in this library.
 *
 */
class Exception : public std::exception {
private:
  int _errCode;
  std::string _errMsg;

  void setLastErrorMessage() {
    if (_errMsg.size() > 0) {
      setLastErrMsg(_errMsg.c_str(), _errMsg.size() + 1);
    }
  }

public:
  Exception() : Exception(ERR) {}
  Exception(int errCode) : _errCode(errCode) { setLastErrorMessage(); };
  Exception(int errCode, std::string errMsg)
      : _errCode(errCode), _errMsg(errMsg) {
    setLastErrorMessage();
  };
  virtual ~Exception(){};

  virtual int getErrCode() const { return _errCode; }

  virtual const char *getErrMsg() const {
    if (_errMsg.size() > 0) {
      return _errMsg.c_str();
    } else {
      return errMsg(_errCode);
    }
  }

  virtual const char *what() const noexcept override {
    return "General exception from LEVEL. For more information on what went "
           "wrong use methods getErrCode() or getErrMsg().";
  }

  /**
   * @brief Returns a general description for an error code.
   *
   * @param errCode \ref level_error.h "Error code"
   * @return const char* Description for the error code.
   */
  static const char *errMsg(int errCode) {
    switch (errCode) {
    case ERR:
      return "General error code.";
    case ERR_MSG_NOT_FOUND:
      return "Message was not found.";
    case ERR_ALLOC_FAILED:
      return "Memory allocation failed.";
    case ERR_NULL:
      return "Null pointer.";
    case ERR_BUFFER_OVERFLOW:
      return "Buffer is too small.";
    case ERR_ENCODE:
      return "Encoding failed.";
    case ERR_DECODE:
      return "Decoding failed.";
    case ERR_RECEIVER_START:
      return "Starting the receiver failed.";
    case ERR_TRANSMITTER_START:
      return "Starting the transmitter failed.";
    case ERR_ARG_NULL:
      return "Argument was null.";
    case ERR_INDEX_OUT_OF_RANGE:
      return "Index is out of range.";
    case ERR_INVALID_ARG:
      return "Provided argument is invalid.";
    case ERR_INVALID_OPERATION:
      return "Detected an invalid operation.";
    case ERR_HIGH_FREQ_CONTAINER_TYPE:
      return "Wrong Type of HighFrequencyContainer.";
    case ERR_LOW_FREQ_CONTAINER_TYPE:
      return "Wrong Type of LowFrequencyContainer.";
    case ERR_CAM_ALREADY_EXISTS:
      return "A CAM with this ID already exists.";
    case ERR_SPECIAL_VEHICLE_CONTAINER_TYPE:
      return "Wrong Type of SpecialVehicleContainer.";
    case ERR_DENM_ALREADY_EXISTS:
      return "A DENM with this ID and SequenceNumber already exists.";
    default:
      return "Unknown error code.";
    }
  }
};

} // namespace level
