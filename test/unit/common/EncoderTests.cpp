#include <CAM.h>
#include <DEREncoder.hpp>
#include <IEncoder.hpp>
#include <XEREncoder.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace level::EncoderTests {

void freeMsg(CAM *msg) { ASN_STRUCT_FREE(asn_DEF_CAM, msg); }

std::shared_ptr<IEncoder<CAM>> getXEREncoder(bool canonical = false) {
  xer_encoder_flags_e encoding = canonical ? XER_F_CANONICAL : XER_F_BASIC;
  return std::make_shared<XEREncoder<CAM>>(asn_DEF_CAM, encoding);
}

std::shared_ptr<IEncoder<CAM>> getDEREncoder() {
  return std::make_shared<DEREncoder<CAM>>(asn_DEF_CAM);
}

CAM *getMsg(bool valid = true) {
  auto cam = (CAM *)calloc(1, sizeof(CAM));
  cam->header.messageID = rand();
  cam->header.stationID = rand();
  cam->header.protocolVersion = rand();
  auto bc = &cam->cam.camParameters.basicContainer;
  bc->referencePosition.latitude = rand();
  bc->referencePosition.longitude = rand();
  if (valid) {
    cam->cam.camParameters.highFrequencyContainer.present =
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;
    auto bvchf = &cam->cam.camParameters.highFrequencyContainer.choice
                      .basicVehicleContainerHighFrequency;
    bvchf->speed.speedValue = rand() % 16000;
    bvchf->driveDirection = DriveDirection_backward;
  }
  return cam;
}

bool equal(CAM *cam1, CAM *cam2) {
  if (!cam1 || !cam2) {
    return false;
  }

  bool result = true;
  result &= cam1->header.stationID == cam2->header.stationID;
  result &= cam1->cam.camParameters.highFrequencyContainer.present ==
            cam2->cam.camParameters.highFrequencyContainer.present;
  if (cam1->cam.camParameters.highFrequencyContainer.present ==
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency) {
    result &= cam1->cam.camParameters.highFrequencyContainer.choice
                  .basicVehicleContainerHighFrequency.speed.speedValue ==
              cam2->cam.camParameters.highFrequencyContainer.choice
                  .basicVehicleContainerHighFrequency.speed.speedValue;
  }
  return result;
}

bool isValidXEREncode(std::vector<BYTE> data) {
  std::string dataStart({(char)data[0], (char)data[1], (char)data[2],
                         (char)data[3], (char)data[4]});
  int n = data.size();
  if (data[n - 1] == '\n') {
    n--;
  }
  std::string dataEnd({(char)data[n - 6], (char)data[n - 5], (char)data[n - 4],
                       (char)data[n - 3], (char)data[n - 2],
                       (char)data[n - 1]});
  return "<CAM>" == dataStart && "</CAM>" == dataEnd;
}

bool isValidDEREncode(std::vector<BYTE> data) {
  assert(data.size() <= 129 && "Not implemented.");
  return data[0] == 48 && data[1] == (data.size() - 2);
}

} // namespace level::EncoderTests

using namespace level;
using namespace level::EncoderTests;

TEST(Encoder, XEREncode) {
  auto encoder = getXEREncoder();
  auto msg = getMsg();
  auto data = encoder->encode(msg);
  ASSERT_TRUE(isValidXEREncode(data));
  freeMsg(msg);
}

TEST(Encoder, XEREncodeCanonical) {
  auto encoder = getXEREncoder(true);
  auto msg = getMsg();
  auto data = encoder->encode(msg);
  ASSERT_TRUE(isValidXEREncode(data));
  freeMsg(msg);
}

TEST(Encoder, XEREncodeFailed) {
  auto encoder = getXEREncoder();
  auto msg = getMsg(false);
  ASSERT_THROW(encoder->encode(msg), EncodeException);
  freeMsg(msg);
}

TEST(Encoder, XEREncodeNullptr) {
  auto encoder = getXEREncoder();
  ASSERT_THROW(encoder->encode(nullptr), EncodeException);
}

TEST(Encoder, XERDecode) {
  auto encoder = getXEREncoder();
  auto msgIn = getMsg();
  auto data = encoder->encode(msgIn);
  auto msgOut = encoder->decode(data);
  ASSERT_TRUE(equal(msgIn, msgOut));
  freeMsg(msgOut);
  freeMsg(msgIn);
}

TEST(Encoder, XERDecodeCanonical) {
  auto encoder = getXEREncoder(true);
  auto msgIn = getMsg();
  auto data = encoder->encode(msgIn);
  auto msgOut = encoder->decode(data);
  ASSERT_TRUE(equal(msgIn, msgOut));
  freeMsg(msgOut);
  freeMsg(msgIn);
}

TEST(Encoder, XERDecodeFailed) {
  auto encoder = getXEREncoder(true);
  auto msgIn = getMsg();
  auto data = encoder->encode(msgIn);
  for (size_t i = data.size() - 32; i < data.size(); i++) {
    data[i] = rand() % 256;
  }
  ASSERT_THROW(encoder->decode(data), EncodeException);
  freeMsg(msgIn);
}

TEST(Encoder, XERDecodeEmptyVector) {
  auto encoder = getXEREncoder(true);
  auto data = std::vector<BYTE>();
  ASSERT_THROW(encoder->decode(data), EncodeException);
}

TEST(Encoder, XERDecodeNullptr) {
  auto encoder = getXEREncoder(true);
  ASSERT_THROW(encoder->decode(nullptr, 0), EncodeException);
}

TEST(Encoder, DEREncode) {
  auto encoder = getDEREncoder();
  auto msg = getMsg();
  auto data = encoder->encode(msg);
  ASSERT_TRUE(isValidDEREncode(data));
  freeMsg(msg);
}

TEST(Encoder, DEREncodeFailed) {
  auto encoder = getDEREncoder();
  auto msg = getMsg(false);
  ASSERT_THROW(encoder->encode(msg), EncodeException);
  freeMsg(msg);
}

TEST(Encoder, DEREncodeNullptr) {
  auto encoder = getDEREncoder();
  ASSERT_THROW(encoder->encode(nullptr), Exception);
}

TEST(Encoder, DERDecode) {
  auto encoder = getDEREncoder();
  auto msgIn = getMsg();
  auto data = encoder->encode(msgIn);
  auto msgOut = encoder->decode(data);
  ASSERT_TRUE(equal(msgIn, msgOut));
  freeMsg(msgOut);
  freeMsg(msgIn);
}

TEST(Encoder, DERDecodeFailed) {
  auto encoder = getDEREncoder();
  auto msgIn = getMsg();
  auto data = encoder->encode(msgIn);
  for (size_t i = data.size() - 32; i < data.size(); i++) {
    data[i] = rand() % 256;
  }
  ASSERT_THROW(encoder->decode(data), EncodeException);
  freeMsg(msgIn);
}

TEST(Encoder, DERDecodeEmptyVector) {
  auto encoder = getDEREncoder();
  auto data = std::vector<BYTE>();
  ASSERT_THROW(encoder->decode(data), EncodeException);
}

TEST(Encoder, DERDecodeNullptr) {
  auto encoder = getDEREncoder();
  ASSERT_THROW(encoder->decode(nullptr, 0), EncodeException);
}