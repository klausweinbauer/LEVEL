#include <CAM.h>
#include <IEncoder.hpp>
#include <XEREncoder.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace level::EncoderTests {

std::shared_ptr<IEncoder<CAM>> getEncoder(bool canonical = false) {
  xer_encoder_flags_e encoding = canonical ? XER_F_CANONICAL : XER_F_BASIC;
  return std::make_shared<XEREncoder<CAM>>(asn_DEF_CAM, encoding);
}

std::shared_ptr<CAM> getMsg(bool valid = true) {
  auto cam = std::make_shared<CAM_t>();
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

bool equal(std::shared_ptr<CAM> cam1, std::shared_ptr<CAM> cam2) {
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

bool isValidEncode(std::vector<BYTE> data) {
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

} // namespace level::EncoderTests

using namespace level;
using namespace level::EncoderTests;

TEST(XEREncoder, Encode) {
  auto encoder = getEncoder();
  auto msg = getMsg();
  auto data = encoder->encode(msg);
  ASSERT_TRUE(isValidEncode(data));
}

TEST(XEREncoder, EncodeCanonical) {
  auto encoder = getEncoder(true);
  auto msg = getMsg();
  auto data = encoder->encode(msg);
  ASSERT_TRUE(isValidEncode(data));
}

TEST(XEREncoder, EncodeFailed) {
  auto encoder = getEncoder();
  auto msg = getMsg(false);
  ASSERT_THROW(encoder->encode(msg), EncodeException);
}

TEST(XEREncoder, EncodeNullptr) {
  auto encoder = getEncoder();
  ASSERT_THROW(encoder->encode(nullptr), EncodeException);
}

TEST(XEREncoder, Decode) {
  auto encoder = getEncoder();
  auto msgIn = getMsg();
  auto data = encoder->encode(msgIn);
  auto msgOut = encoder->decode(data);
  ASSERT_TRUE(equal(msgIn, msgOut));
}

TEST(XEREncoder, DecodeCanonical) {
  auto encoder = getEncoder(true);
  auto msgIn = getMsg();
  auto data = encoder->encode(msgIn);
  auto msgOut = encoder->decode(data);
  ASSERT_TRUE(equal(msgIn, msgOut));
}

TEST(XEREncoder, DecodeFailed) {
  auto encoder = getEncoder(true);
  auto msgIn = getMsg();
  auto data = encoder->encode(msgIn);
  for (size_t i = data.size() - 32; i < data.size(); i++) {
    data[i] = rand() % 256;
  }
  ASSERT_THROW(encoder->decode(data), EncodeException);
}

TEST(XEREncoder, DecodeEmptyVector) {
  auto encoder = getEncoder(true);
  auto data = std::vector<BYTE>();
  ASSERT_THROW(encoder->decode(data), EncodeException);
}

TEST(XEREncoder, DecodeNullptr) {
  auto encoder = getEncoder(true);
  ASSERT_THROW(encoder->decode(nullptr, 0), EncodeException);
}