#pragma once

#include <Exception.hpp>
#include <level_error.h>
#include <string>

namespace level {

class EncodeException : public Exception {
public:
  EncodeException(std::string encodingType, std::string messageType) {
    std::stringstream stream;
    stream << "Message de-/encoding failed. (Encoding: " << encodingType
           << ", Message: " << messageType << ")" << std::endl;
    Exception(ERR_ENCODE, stream.str());
  }

  EncodeException(std::string encodingType, std::string messageType,
                  std::string messageProperty, std::string errorType) {
    std::stringstream stream;
    stream << encodingType << " encoding failed for message " << messageType
           << " with error type '" << errorType
           << "'. This is probably due to an invalid value of property '"
           << messageProperty << "'." << std::endl;
    Exception(ERR_ENCODE, stream.str());
  }
};

} // namespace level