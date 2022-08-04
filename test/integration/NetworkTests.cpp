#include <CAMEncoder.hpp>
#include <CAMNetworkInterface.hpp>
#include <CAMWrapper.hpp>
#include <DENMEncoder.hpp>
#include <DENMNetworkInterface.hpp>
#include <DENMWrapper.hpp>
#include <Syscall.hpp>
#include <UDPSocket.hpp>
#include <gtest/gtest.h>
#include <memory>

using namespace level;

std::shared_ptr<ISyscall> sys() { return std::make_shared<Syscall>(); }

TEST(Network, SendAndReceiveCAMsWith100Hz) {
  using namespace level::cam;

  const int msgsToSend = 100;
  bool recvFlags[msgsToSend] = {false};
  int errCount = 0;
  int recvMsgCount = 0;

  std::function<void(CAM *)> recvCallback = [&recvMsgCount,
                                             &recvFlags](CAM *msg) {
    cam::CAMWrapper cam(msg);
    int index = cam->header.stationID;
    recvFlags[index] = true;
    recvMsgCount++;
  };

  std::function<void(const Exception &)> errCallback =
      [&errCount](const Exception &e) { errCount++; };

  unsigned int port = 50000 + rand() % 10000;
  cam::CAMNetworkInterface nal(std::make_shared<UDPSocket>(port, sys()),
                               std::make_shared<UDPSocket>(port, sys()),
                               std::make_shared<CAMDEREncoder>(), recvCallback,
                               errCallback);

  int sendCount = 0;
  do {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    cam::CAMWrapper cam(
        sendCount,
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
    nal.send(cam.get());
    sendCount++;
  } while (sendCount < msgsToSend);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_EQ(msgsToSend, recvMsgCount);
  for (int i = 0; i < msgsToSend; i++) {
    ASSERT_TRUE(recvFlags[i]);
  }
}

TEST(Network, SendAndReceiveDENMsWith100Hz) {
  const int msgsToSend = 100;
  bool recvFlags[msgsToSend] = {false};
  int errCount = 0;
  int recvMsgCount = 0;

  std::function<void(DENM *)> recvCallback = [&recvMsgCount,
                                              &recvFlags](DENM *msg) {
    denm::DENMWrapper denm(msg);
    int index = denm->header.stationID;
    recvFlags[index] = true;
    recvMsgCount++;
  };

  std::function<void(const Exception &)> errCallback =
      [&errCount](const Exception &e) { errCount++; };

  unsigned int port = 50000 + rand() % 10000;
  denm::DENMNetworkInterface nal(std::make_shared<UDPSocket>(port, sys()),
                                 std::make_shared<UDPSocket>(port, sys()),
                                 std::make_shared<denm::DEREncoder>(),
                                 recvCallback, errCallback);

  int sendCount = 0;
  do {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    denm::DENMWrapper denm(
        sendCount,
        HighFrequencyContainer_PR_basicVehicleContainerHighFrequency);
    nal.send(denm.get());
    sendCount++;
  } while (sendCount < msgsToSend);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_EQ(msgsToSend, recvMsgCount);
  for (int i = 0; i < msgsToSend; i++) {
    ASSERT_TRUE(recvFlags[i]);
  }
}