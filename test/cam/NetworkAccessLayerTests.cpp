#include <CAM.h>
#include <CAMFactory.hpp>
#include <NetworkException.hpp>
#include <PacketReceiver.hpp>
#include <UDPSocket.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace level;
using namespace level::cam;

static CAM_t *testSendAndReceiveMessage_cam = nullptr;
static void recvPacket(CAM_t *cam) { testSendAndReceiveMessage_cam = cam; }

TEST(CAM_NetworkAccessLayer, Test_Send_And_Receive_Message) {
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  auto id = 97;
  cam->header.stationID = id;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;

  Factory::networkAL().recvCallback = recvPacket;
  Factory::networkAL().send(cam);

  while (!testSendAndReceiveMessage_cam) {
    usleep(1);
  }

  ASSERT_EQ(id, testSendAndReceiveMessage_cam->header.stationID);

  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
  ASN_STRUCT_FREE(asn_DEF_CAM, testSendAndReceiveMessage_cam);
}