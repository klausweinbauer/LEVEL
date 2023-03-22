// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

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