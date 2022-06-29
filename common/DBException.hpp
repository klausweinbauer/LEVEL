#pragma once

#include <exception>
#include <string>

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

  int getErrorCode() { return _errorCode; }
  std::string getErrorMessage() { return _msg; }
};

} // namespace c2x
