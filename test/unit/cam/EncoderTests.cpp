#include <CAM.h>
#include <CAMServiceProvider.hpp>
#include <Encoder.hpp>
#include <Exception.hpp>
#include <IEncoder.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;
using namespace level::cam;

std::shared_ptr<IEncoder>
getEncoder(EncodingType encoding = EncodingType::DER_BER) {
  return std::shared_ptr<IEncoder>(new Encoder(encoding));
}

TEST(CAMEncoder, TestCAMencodingBER) {
  auto encoder = getEncoder(EncodingType::DER_BER);
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->header.stationID = 1;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  cam->cam.camParameters.highFrequencyContainer.choice
      .basicVehicleContainerHighFrequency.speed.speedValue = 1000;
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  int len = encoder->encode(cam, buffer, bufferLen);
  CAM_t *camDecode = encoder->decode(buffer, len);

  ASSERT_LT(0, len);
  ASSERT_GT(1024, len);
  ASSERT_EQ(cam->header.stationID, camDecode->header.stationID);
  ASSERT_EQ(cam->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.speed.speedValue,
            camDecode->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.speed.speedValue);

  free(buffer);
  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
  ASN_STRUCT_FREE(asn_DEF_CAM, camDecode);
}

TEST(CAMEncoder, TestCAMencodingXERCANONICAL) {
  auto encoder = getEncoder(EncodingType::XER_CANONICAL);
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->header.stationID = 1;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  cam->cam.camParameters.highFrequencyContainer.choice
      .basicVehicleContainerHighFrequency.speed.speedValue = 1000;
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  int len = encoder->encode(cam, buffer, bufferLen);
  CAM_t *camDecode = encoder->decode(buffer, len);

  ASSERT_LT(0, len);
  ASSERT_GT(2096, len);
  ASSERT_EQ(cam->header.stationID, camDecode->header.stationID);
  ASSERT_EQ(cam->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.speed.speedValue,
            camDecode->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.speed.speedValue);

  free(buffer);
  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
  ASN_STRUCT_FREE(asn_DEF_CAM, camDecode);
}

TEST(CAMEncoder, TestCAMencodingXERBASIC) {
  auto encoder = getEncoder(EncodingType::XER_BASIC);
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->header.stationID = 1;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  cam->cam.camParameters.highFrequencyContainer.choice
      .basicVehicleContainerHighFrequency.speed.speedValue = 1000;
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  int len = encoder->encode(cam, buffer, bufferLen);
  CAM_t *camDecode = encoder->decode(buffer, len);

  ASSERT_LT(0, len);
  ASSERT_GT(4096, len);
  ASSERT_EQ(cam->header.stationID, camDecode->header.stationID);
  ASSERT_EQ(cam->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.speed.speedValue,
            camDecode->cam.camParameters.highFrequencyContainer.choice
                .basicVehicleContainerHighFrequency.speed.speedValue);

  free(buffer);
  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
  ASN_STRUCT_FREE(asn_DEF_CAM, camDecode);
}

TEST(CAMEncoder, TestEncodeWithInvalidType) {
  auto encoder = getEncoder(EncodingType(-1));
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);
  int errCode;

  try {
    encoder->encode(cam, buffer, bufferLen);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_INVALID_ARG, errCode);

  free(buffer);
  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
}

TEST(CAMEncoder, TestEncodeWithNullptr) {
  auto encoder = getEncoder();
  int errCode = 0;
  auto buffer = new uint8_t[32];

  try {
    encoder->encode(nullptr, buffer, 32);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_ARG_NULL, errCode);

  delete[] buffer;
}

TEST(CAMEncoder, TestEncodeInvalidCAM) {
  auto encoder = getEncoder();
  int errCode = 0;
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));

  try {
    encoder->encode(cam, buffer, bufferLen);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_ENCODE, errCode);

  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
  free(buffer);
}

TEST(CAMEncoder, TestEncodeWithTooSmallBuffer) {
  auto encoder = getEncoder();
  int errCode = 0;
  int bufferLen = 32;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;

  try {
    encoder->encode(cam, buffer, bufferLen);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_BUFFER_OVERFLOW, errCode);

  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
  free(buffer);
}

TEST(CAMEncoder, TestDecodeWithNullptr) {
  auto encoder = getEncoder();
  int errCode = 0;

  try {
    encoder->decode(nullptr, 0);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_ARG_NULL, errCode);
}

TEST(CAMEncoder, TestDecodeWithEmptyBuffer) {
  auto encoder = getEncoder();
  int errCode = 0;
  auto buffer = new uint8_t[0];

  try {
    encoder->decode(buffer, 0);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_ARG_NULL, errCode);

  delete[] buffer;
}

TEST(CAMEncoder, TestDecodeWithInvalidData) {
  auto encoder = getEncoder(EncodingType::DER_BER);
  int errCode = 0;
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  int bufferLen = 128;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  int len = encoder->encode(cam, buffer, bufferLen);
  for (size_t i = 16; i < 32; i++) {
    buffer[i] = 0;
  }

  try {
    encoder->decode(buffer, len);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_DECODE, errCode);

  free(buffer);
  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
}

TEST(CAMEncoder, TestDecodeWithInvalidType) {
  auto encoder = getEncoder(EncodingType(-1));
  int errCode = 0;
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  try {
    encoder->decode(buffer, bufferLen);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_INVALID_ARG, errCode);

  free(buffer);
  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
}

TEST(CAMEncoder, TestEncodePropertyFailed) {
  auto encoder = getEncoder(EncodingType::DER_BER);
  int errCode = 0;
  std::string errMsg;
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  auto lfc =
      (LowFrequencyContainer_t *)calloc(1, sizeof(LowFrequencyContainer_t));
  cam->cam.camParameters.lowFrequencyContainer = lfc;
  lfc->choice.basicVehicleContainerLowFrequency.vehicleRole = 99;
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  try {
    encoder->encode(cam, buffer, bufferLen);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
    errMsg.assign(e.getErrMsg());
  }

  ASSERT_EQ(ERR_ENCODE, errCode);
  ASSERT_THAT(errMsg, ::testing::HasSubstr("LowFrequencyContainer"));

  free(buffer);
  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
}