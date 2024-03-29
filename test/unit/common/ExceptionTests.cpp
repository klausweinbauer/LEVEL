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

#include <EncodeException.hpp>
#include <Exception.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace level;

const char *defaultErrorMsg = "Unknown error code.";

TEST(Exception, SetLastErrorMessage) {
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

TEST(Exception, ErrorMessageForUnknownError) {
  std::string msg(Exception::errMsg(1));
  ASSERT_GT(msg.length(), 0);
  ASSERT_FALSE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, GetDefaultErrorMessage) {
  Exception ex(ERR);
  ASSERT_EQ(std::string(Exception::errMsg(ERR)), std::string(ex.getErrMsg()));
}

TEST(Exception, GetDefaultErrorCode) {
  Exception ex;
  ASSERT_EQ(ERR, ex.getErrCode());
}

TEST(Exception, GetExceptionDescription) {
  std::string expMsg(
      "General exception from LEVEL. For more information on what went "
      "wrong use members getErrCode() and getErrMsg() or global "
      "getLastErrMsg() function.");
  Exception ex;
  std::string msg(ex.what());
  ASSERT_EQ(expMsg, msg);
}

TEST(Exception, DefaultEncodeErrorMessage) {
  std::string expMsg("Message de-/encoding failed.");
  EncodeException e;
  ASSERT_EQ(expMsg, std::string(e.getErrMsg()));
}

TEST(Exception, InitOnlyWithCode) {
  std::string expMsg("Null pointer.");
  Exception e(ERR_NULL);
  ASSERT_EQ(expMsg, std::string(e.getErrMsg()));
  ASSERT_EQ(expMsg, std::string(e.what()));
}

TEST(Exception, ErrorMessageERR) {
  std::string msg(Exception::errMsg(ERR));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRALLOCFAILED) {
  std::string msg(Exception::errMsg(ERR_ALLOC_FAILED));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRNULL) {
  std::string msg(Exception::errMsg(ERR_NULL));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRBUFFEROVERFLOW) {
  std::string msg(Exception::errMsg(ERR_BUFFER_OVERFLOW));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRENCODE) {
  std::string msg(Exception::errMsg(ERR_ENCODE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRDECODE) {
  std::string msg(Exception::errMsg(ERR_DECODE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRRECEIVERSTART) {
  std::string msg(Exception::errMsg(ERR_RECEIVER_START));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRTRANSMITTERSTART) {
  std::string msg(Exception::errMsg(ERR_TRANSMITTER_START));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRARGNULL) {
  std::string msg(Exception::errMsg(ERR_ARG_NULL));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRINDEXOUTOFRANGE) {
  std::string msg(Exception::errMsg(ERR_INDEX_OUT_OF_RANGE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRINVALIDARG) {
  std::string msg(Exception::errMsg(ERR_INVALID_ARG));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRMSGNOTFOUND) {
  std::string msg(Exception::errMsg(ERR_MSG_NOT_FOUND));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRINVALIDOPERATION) {
  std::string msg(Exception::errMsg(ERR_INVALID_OPERATION));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRHIGHFREQCONTAINERTYPE) {
  std::string msg(Exception::errMsg(ERR_HIGH_FREQ_CONTAINER_TYPE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRLOWFREQCONTAINERTYPE) {
  std::string msg(Exception::errMsg(ERR_LOW_FREQ_CONTAINER_TYPE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRCAMALREADYEXISTS) {
  std::string msg(Exception::errMsg(ERR_CAM_ALREADY_EXISTS));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRSPECIALVEHICLECONTAINERTYPE) {
  std::string msg(Exception::errMsg(ERR_SPECIAL_VEHICLE_CONTAINER_TYPE));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}

TEST(Exception, ErrorMessageERRDENMALREADYEXISTS) {
  std::string msg(Exception::errMsg(ERR_DENM_ALREADY_EXISTS));
  ASSERT_GT(msg.length(), 0);
  ASSERT_TRUE(msg.rfind(defaultErrorMsg, 0));
}
