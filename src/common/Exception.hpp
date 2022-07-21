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

public:
  Exception(int errCode) : _errCode(errCode){};
  Exception(int errCode, std::string errMsg)
      : _errCode(errCode), _errMsg(errMsg){};
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
    std::stringstream ss;

    switch (errCode) {
    case ERR_MSG_NOT_FOUND:
      ss << "Message was not found." << std::endl;
      break;
    case ERR_ALLOC_FAILED:
      ss << "Memory allocation failed." << std::endl;
      break;
    case ERR_NULL:
      ss << "Null pointer." << std::endl;
      break;
    case ERR_BUFFER_OVERFLOW:
      ss << "Buffer is too small." << std::endl;
      break;
    case ERR_ENCODE:
      ss << "Encoding failed." << std::endl;
      break;
    case ERR_DECODE:
      ss << "Decoding failed." << std::endl;
      break;
    case ERR_RECEIVER_START:
      ss << "Starting the receiver failed." << std::endl;
      break;
    case ERR_TRANSMITTER_START:
      ss << "Starting the transmitter failed." << std::endl;
      break;
    case ERR_ARG_NULL:
      ss << "Argument was null." << std::endl;
      break;
    case ERR_INDEX_OUT_OF_RANGE:
      ss << "Index is out of range." << std::endl;
      break;
    case ERR_INVALID_OPERATION:
      ss << "Detected an invalid operation." << std::endl;
      break;
    case ERR_HIGH_FREQ_CONTAINER_TYPE:
      ss << "Wrong Type of HighFrequencyContainer." << std::endl;
      break;
    case ERR_LOW_FREQ_CONTAINER_TYPE:
      ss << "Wrong Type of LowFrequencyContainer." << std::endl;
      break;
    case ERR_CAM_ALREADY_EXISTS:
      ss << "A CAM with this ID already exists." << std::endl;
      break;
    case ERR_SPECIAL_VEHICLE_CONTAINER_TYPE:
      ss << "Wrong Type of SpecialVehicleContainer." << std::endl;
      break;
    case ERR_DENM_ALREADY_EXISTS:
      ss << "A DENM with this ID and SequenceNumber already exists."
         << std::endl;
      break;
    default:
      ss << "Unknown error code '" << errCode << "'." << std::endl;
      break;
    }

    return ss.str().c_str();
  }
};

} // namespace level
