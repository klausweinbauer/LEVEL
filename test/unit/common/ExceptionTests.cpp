#include <Exception.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace level;

const char *defaultErrorMsg = "Unknown error code.";

TEST(Exception, TestSetLastErrorMessage) {
  std::string expMsg("Test error message.");

  try {
    throw Exception(ERR, expMsg);
  } catch (const std::exception &) {
  }

  char buffer[256];
  int size;
  getLastErrMsg(buffer, 256, &size);
  std::string msg(buffer);

  ASSERT_EQ(0, expMsg.compare(msg));
}

TEST(Exception, TestErrorMessageForUnknownError) {
  std::string msg(Exception::errMsg(1));
  ASSERT_GT(msg.length(), 0);
  ASSERT_FALSE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestGetDefaultErrorMessage) {
  Exception ex(ERR);
  ASSERT_EQ(Exception::errMsg(ERR), ex.getErrMsg());
}

TEST(Exception, TestGetExceptionDescription) {
  std::string expMsg(
      "General exception from LEVEL. For more information on what went "
      "wrong use members getErrCode() and getErrMsg() or global "
      "getLastErrMsg() function.");
  Exception ex;
  std::string msg(ex.what());
  ASSERT_EQ(expMsg, msg);
}

TEST(Exception, TestErrorMessageERR) {
  std::string msg(Exception::errMsg(ERR));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRALLOCFAILED) {
  std::string msg(Exception::errMsg(ERR_ALLOC_FAILED));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRNULL) {
  std::string msg(Exception::errMsg(ERR_NULL));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRBUFFEROVERFLOW) {
  std::string msg(Exception::errMsg(ERR_BUFFER_OVERFLOW));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRENCODE) {
  std::string msg(Exception::errMsg(ERR_ENCODE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRDECODE) {
  std::string msg(Exception::errMsg(ERR_DECODE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRRECEIVERSTART) {
  std::string msg(Exception::errMsg(ERR_RECEIVER_START));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRTRANSMITTERSTART) {
  std::string msg(Exception::errMsg(ERR_TRANSMITTER_START));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRARGNULL) {
  std::string msg(Exception::errMsg(ERR_ARG_NULL));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRINDEXOUTOFRANGE) {
  std::string msg(Exception::errMsg(ERR_INDEX_OUT_OF_RANGE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRINVALIDARG) {
  std::string msg(Exception::errMsg(ERR_INVALID_ARG));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRMSGNOTFOUND) {
  std::string msg(Exception::errMsg(ERR_MSG_NOT_FOUND));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRINVALIDOPERATION) {
  std::string msg(Exception::errMsg(ERR_INVALID_OPERATION));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRHIGHFREQCONTAINERTYPE) {
  std::string msg(Exception::errMsg(ERR_HIGH_FREQ_CONTAINER_TYPE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRLOWFREQCONTAINERTYPE) {
  std::string msg(Exception::errMsg(ERR_LOW_FREQ_CONTAINER_TYPE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRCAMALREADYEXISTS) {
  std::string msg(Exception::errMsg(ERR_CAM_ALREADY_EXISTS));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRSPECIALVEHICLECONTAINERTYPE) {
  std::string msg(Exception::errMsg(ERR_SPECIAL_VEHICLE_CONTAINER_TYPE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, TestErrorMessageERRDENMALREADYEXISTS) {
  std::string msg(Exception::errMsg(ERR_DENM_ALREADY_EXISTS));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}
