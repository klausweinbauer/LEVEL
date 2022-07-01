#include <CAM.h>
#include <Encoder.hpp>
#include <Exception.hpp>
#include <Factory.hpp>
#include <IEncoder.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace c2x;
using namespace c2x::cam;

std::shared_ptr<IEncoder> getEncoder() {
  return std::shared_ptr<IEncoder>(new Encoder());
}

TEST(EncoderTests, Test_Factory_Function) {
  ASSERT_NE(nullptr, &Factory::encoder());
}

TEST(EncoderTests, Test_CAM_encoding_BER) {
  auto encoder = getEncoder();
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->header.stationID = 1;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  cam->cam.camParameters.highFrequencyContainer.choice
      .basicVehicleContainerHighFrequency.speed.speedValue = 1000;
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  int len = encoder->encode(cam, buffer, bufferLen, EncodingType::DER_BER);
  CAM_t *camDecode = encoder->decode(buffer, len, EncodingType::DER_BER);

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

TEST(EncoderTests, Test_CAM_encoding_XER_CANONICAL) {
  auto encoder = getEncoder();
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->header.stationID = 1;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  cam->cam.camParameters.highFrequencyContainer.choice
      .basicVehicleContainerHighFrequency.speed.speedValue = 1000;
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  int len =
      encoder->encode(cam, buffer, bufferLen, EncodingType::XER_CANONICAL);
  CAM_t *camDecode = encoder->decode(buffer, len, EncodingType::XER_CANONICAL);

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

TEST(EncoderTests, Test_CAM_encoding_XER_BASIC) {
  auto encoder = getEncoder();
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->header.stationID = 1;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  cam->cam.camParameters.highFrequencyContainer.choice
      .basicVehicleContainerHighFrequency.speed.speedValue = 1000;
  int bufferLen = 4096;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  int len = encoder->encode(cam, buffer, bufferLen, EncodingType::XER_BASIC);
  CAM_t *camDecode = encoder->decode(buffer, len, EncodingType::XER_BASIC);

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

TEST(EncoderTests, Test_Encode_With_Nullptr) {
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

TEST(EncoderTests, Test_Encode_Invalid_CAM) {
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

TEST(EncoderTests, Test_Encode_With_Too_Small_Buffer) {
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

TEST(EncoderTests, Test_Decode_With_Nullptr) {
  auto encoder = getEncoder();
  int errCode = 0;

  try {
    encoder->decode(nullptr, 0);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_ARG_NULL, errCode);
}

TEST(EncoderTests, Test_Decode_With_Empty_Buffer) {
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

TEST(EncoderTests, Test_Decode_With_Invalid_Data) {
  auto encoder = getEncoder();
  int errCode = 0;
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
  int bufferLen = 128;
  uint8_t *buffer = (uint8_t *)calloc(1, bufferLen);

  int len = encoder->encode(cam, buffer, bufferLen, EncodingType::DER_BER);
  for (size_t i = 16; i < 32; i++) {
    buffer[i] = 0;
  }

  try {
    encoder->decode(buffer, len, EncodingType::DER_BER);
  } catch (const Exception &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR_DECODE, errCode);

  free(buffer);
  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
}