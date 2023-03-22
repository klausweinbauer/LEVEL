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
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string.h>
#include <time.h>

using namespace level;

int main(int argc, char **argv) {
  srand(time(0));

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);
  int ret = RUN_ALL_TESTS();
  clearErrMsg();
  return ret;
}

TEST(API, TestGetVersion) {
  auto version = getVersion();
  ASSERT_TRUE(0 == strcmp("0.1.0", version));
}

TEST(API, TestGetErrorMessage) {
  std::string expMsg("General error code.");

  char buffer[256];
  getErrMsg(ERR, buffer, 256);
  std::string msg(buffer);

  ASSERT_EQ(0, expMsg.compare(msg));
}

TEST(API, TestSetErrorMessage) {
  std::string expMsg("Test_Set_Error_Message message.");
  char buffer[256];
  int size;

  Exception::setErrMsg(expMsg.c_str(), expMsg.size() + 1);
  getLastErrMsg(buffer, 256, &size);

  std::string msg(buffer);

  ASSERT_EQ(0, expMsg.compare(msg));
}

TEST(API, TestSetInvalidErrorMessage) {
  std::string expMsg("Test_Set_Invalid_Error_Message message.");
  char buffer[256];
  int size;

  Exception::setErrMsg(expMsg.c_str(), expMsg.size() + 1);
  Exception::setErrMsg(nullptr, expMsg.size() + 1);
  getLastErrMsg(buffer, 256, &size);

  std::string msg(buffer);

  ASSERT_EQ(0, expMsg.compare(msg));
}

TEST(API, TestOverrideErrorMessage) {
  std::string oldMsg("Test_Override_Error_Message message old.");
  std::string expMsg("Test_Override_Error_Message message.");
  char buffer[256];
  int size;

  Exception::setErrMsg(oldMsg.c_str(), oldMsg.size() + 1);
  Exception::setErrMsg(expMsg.c_str(), expMsg.size() + 1);
  getLastErrMsg(buffer, 256, &size);

  std::string msg(buffer);

  ASSERT_EQ(0, expMsg.compare(msg));
}

TEST(API, TestGetEmptyLastErrorMessage) {
  char buffer[256];
  int size;

  clearErrMsg();
  getLastErrMsg(buffer, 256, &size);

  ASSERT_EQ(0, size);
}