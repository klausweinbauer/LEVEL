#include <CAM.h>
#include <CAMFactory.hpp>
#include <Mocks.hpp>
#include <NetworkException.hpp>
#include <PacketReceiver.hpp>
#include <UDPSocket.hpp>
#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace level;
using namespace level::cam;

using namespace ::testing;

static CAM_t *testSendAndReceiveMessage_cam = nullptr;
static void recvPacket(CAM_t *cam) { testSendAndReceiveMessage_cam = cam; }

TEST(CAM_NetworkAccessLayer, Test_Send_And_Receive_Message) {
  CAM_t *cam = (CAM_t *)calloc(1, sizeof(CAM_t));
  auto id = 97;
  cam->header.stationID = id;
  cam->cam.camParameters.highFrequencyContainer.present =
      HighFrequencyContainer_PR_basicVehicleContainerHighFrequency;

  Factory::networkAL()->recvCallback = recvPacket;
  Factory::networkAL()->send(cam);

  while (!testSendAndReceiveMessage_cam) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  ASSERT_EQ(id, testSendAndReceiveMessage_cam->header.stationID);

  ASN_STRUCT_FREE(asn_DEF_CAM, cam);
  ASN_STRUCT_FREE(asn_DEF_CAM, testSendAndReceiveMessage_cam);
}

static bool Test_Error_Callback_validCallback = false;
static bool Test_Error_Callback_callbackReceived = false;
void Test_Error_Callback_errCallback(const Exception &e) {
  if (dynamic_cast<const NetworkException *>(&e)) {
    Test_Error_Callback_validCallback = true;
  }
  Test_Error_Callback_callbackReceived = true;
}

TEST(CAM_SocketBasedNAL, Test_Error_Callback) {
  auto sSock = std::shared_ptr<MSocket>(new MSocket());
  auto rSock = std::shared_ptr<MSocket>(new MSocket());

  EXPECT_CALL(*rSock, recvFrom(_, _, _))
      .WillRepeatedly(Throw(NetworkException()));

  SocketBasedNI nal(sSock, rSock, nullptr, nullptr,
                    Test_Error_Callback_errCallback);

  while (!Test_Error_Callback_callbackReceived) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  ASSERT_TRUE(Test_Error_Callback_validCallback);
}