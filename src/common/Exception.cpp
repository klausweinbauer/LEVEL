#include <Exception.hpp>
#include <cstdio>
#include <mutex>
#include <sstream>
#include <string.h>

namespace level {

char *Exception::lastErrorMsg = nullptr;
int Exception::lastErrorMsgSize = 0;
std::mutex Exception::errorMsgLock;

void Exception::setErrMsg(const char *buffer, int size) {
  std::lock_guard<std::mutex> guard(errorMsgLock);

  if (!buffer) {
    return;
  }

  if (lastErrorMsg) {
    lastErrorMsg = (char *)realloc(lastErrorMsg, size);
  } else {
    lastErrorMsg = (char *)malloc(size);
  }

  strncpy(lastErrorMsg, buffer, size);
  lastErrorMsgSize = size;
}

void Exception::getErrMsg(int err, char *buffer, int size) {
  auto msg = Exception::errMsg(err);
  strncpy(buffer, msg, size);
}

void Exception::getLastErrMsg(char *buffer, int size, int *actualSize) {
  std::lock_guard<std::mutex> guard(errorMsgLock);

  if (actualSize) {
    *actualSize = 0;
  }

  if (!lastErrorMsg) {
    return;
  }

  int cpySize = (std::min)(size, lastErrorMsgSize);
  strncpy(buffer, lastErrorMsg, cpySize);

  if (actualSize) {
    *actualSize = cpySize;
  }
}

void Exception::clearErrMsg() {
  std::lock_guard<std::mutex> guard(errorMsgLock);

  if (lastErrorMsg) {
    free(lastErrorMsg);
    lastErrorMsg = nullptr;
    lastErrorMsgSize = 0;
  }
}

} // namespace level