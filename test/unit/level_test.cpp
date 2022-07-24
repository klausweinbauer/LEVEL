#include <Exception.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <level.h>
#include <string.h>
#include <time.h>

using namespace level;

int main(int argc, char **argv) {
  srand(time(0));

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);
  int ret = RUN_ALL_TESTS();
  clearLastErrMsg();
  return ret;
}

TEST(API, Test_Get_Version) {
  auto version = getVersion();
  ASSERT_TRUE(0 == strcmp("0.1.0", version));
}

TEST(API, Test_Get_Error_Message) {
  std::string expMsg("General error code.");

  char buffer[256];
  getErrMsg(ERR, buffer, 256);
  std::string msg(buffer);

  ASSERT_EQ(0, expMsg.compare(msg));
}

TEST(API, Test_Set_Error_Message) {
  std::string expMsg("Test_Set_Error_Message message.");
  char buffer[256];
  int size;

  setLastErrMsg(expMsg.c_str(), expMsg.size() + 1);
  getLastErrMsg(buffer, 256, &size);

  std::string msg(buffer);

  ASSERT_EQ(0, expMsg.compare(msg));
}

TEST(API, Test_Set_Invalid_Error_Message) {
  std::string expMsg("Test_Set_Invalid_Error_Message message.");
  char buffer[256];
  int size;

  setLastErrMsg(expMsg.c_str(), expMsg.size() + 1);
  setLastErrMsg(nullptr, expMsg.size() + 1);
  getLastErrMsg(buffer, 256, &size);

  std::string msg(buffer);

  ASSERT_EQ(0, expMsg.compare(msg));
}

TEST(API, Test_Override_Error_Message) {
  std::string oldMsg("Test_Override_Error_Message message old.");
  std::string expMsg("Test_Override_Error_Message message.");
  char buffer[256];
  int size;

  setLastErrMsg(oldMsg.c_str(), oldMsg.size() + 1);
  setLastErrMsg(expMsg.c_str(), expMsg.size() + 1);
  getLastErrMsg(buffer, 256, &size);

  std::string msg(buffer);

  ASSERT_EQ(0, expMsg.compare(msg));
}

TEST(API, Test_Get_Empty_Last_Error_Message) {
  char buffer[256];
  int size;

  clearLastErrMsg();
  getLastErrMsg(buffer, 256, &size);

  ASSERT_EQ(0, size);
}