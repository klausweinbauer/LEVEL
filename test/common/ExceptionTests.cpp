#include <Exception.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace level;

const char* defaultErrorMsg = "Unknown error code.";

TEST(Common_Exception, Test_Error_Message_For_Unknown_Error) {
  std::string msg(Exception::errMsg(1));
  ASSERT_GT(msg.length(), 0);
  ASSERT_FALSE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR) {
  std::string msg(Exception::errMsg(ERR));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_ALLOC_FAILED) {
  std::string msg(Exception::errMsg(ERR_ALLOC_FAILED));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_NULL) {
  std::string msg(Exception::errMsg(ERR_NULL));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_BUFFER_OVERFLOW) {
  std::string msg(Exception::errMsg(ERR_BUFFER_OVERFLOW));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_ENCODE) {
  std::string msg(Exception::errMsg(ERR_ENCODE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_DECODE) {
  std::string msg(Exception::errMsg(ERR_DECODE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_RECEIVER_START) {
  std::string msg(Exception::errMsg(ERR_RECEIVER_START));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_TRANSMITTER_START) {
  std::string msg(Exception::errMsg(ERR_TRANSMITTER_START));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_ARG_NULL) {
  std::string msg(Exception::errMsg(ERR_ARG_NULL));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_INDEX_OUT_OF_RANGE) {
  std::string msg(Exception::errMsg(ERR_INDEX_OUT_OF_RANGE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_INVALID_ARG) {
  std::string msg(Exception::errMsg(ERR_INVALID_ARG));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_MSG_NOT_FOUND) {
  std::string msg(Exception::errMsg(ERR_MSG_NOT_FOUND));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_INVALID_OPERATION) {
  std::string msg(Exception::errMsg(ERR_INVALID_OPERATION));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_HIGH_FREQ_CONTAINER_TYPE) {
  std::string msg(Exception::errMsg(ERR_HIGH_FREQ_CONTAINER_TYPE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_LOW_FREQ_CONTAINER_TYPE) {
  std::string msg(Exception::errMsg(ERR_LOW_FREQ_CONTAINER_TYPE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_CAM_ALREADY_EXISTS) {
  std::string msg(Exception::errMsg(ERR_CAM_ALREADY_EXISTS));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_SPECIAL_VEHICLE_CONTAINER_TYPE) {
  std::string msg(Exception::errMsg(ERR_SPECIAL_VEHICLE_CONTAINER_TYPE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Common_Exception, Test_Error_Message_ERR_DENM_ALREADY_EXISTS) {
  std::string msg(Exception::errMsg(ERR_DENM_ALREADY_EXISTS));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}
