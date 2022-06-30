#pragma once

#include <c2xcommon.h>
#include <exception>
#include <sstream>
#include <string>

#define DB_EXCEPTION_BUFFER_LEN 256

namespace c2x {
class DBException : public std::exception {
private:
  int _errorCode;
  std::string _msg;

public:
  DBException(int errorCode) : _errorCode(errorCode){};
  DBException(int errorCode, std::string msg)
      : _errorCode(errorCode), _msg(msg){};
  ~DBException(){};

  int getErrorCode() const { return _errorCode; }
  std::string getErrorMessage() const { return _msg; }
  const char *what() const noexcept override {
    if (_msg.size() > 0) {
      return _msg.c_str();
    } else {
      std::stringstream ss;
      ss << "DBException (error code " << _errorCode << ")" << std::endl;
      return ss.str().c_str();
    }
  }
};

} // namespace c2x
