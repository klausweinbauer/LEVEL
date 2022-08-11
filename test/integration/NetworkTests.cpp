#include <CAMEncoder.hpp>
#include <CAMNetworkInterface.hpp>
#include <CAMRecvHandler.hpp>
#include <CAMWrapper.hpp>
#include <DENMEncoder.hpp>
#include <DENMNetworkInterface.hpp>
#include <DENMRecvHandler.hpp>
#include <DENMWrapper.hpp>
#include <ErrorHandler.hpp>
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

  std::function<void(CAM *, bool *)> recvCallback =
      [&recvMsgCount, &recvFlags](CAM *msg, bool *tookOwnership) {
        *tookOwnership = true;
        CAMWrapper cam(msg);
        int index = cam->header.stationID;
        recvFlags[index] = true;
        recvMsgCount++;
      };
  auto recvHandler = std::make_shared<CAMRecvHandler>();
  recvHandler->registerCallback(recvCallback);

  std::function<void(const Exception &)> errCallback =
      [&errCount](const Exception &e) { errCount++; };
  auto errHandler = std::make_shared<ErrorHandler>();
  errHandler->registerCallback(errCallback);

  unsigned int port = 50000 + rand() % 10000;
  CAMNetworkInterface nal(std::make_shared<UDPSocket>(port, sys()),
                          std::make_shared<UDPSocket>(port, sys()),
                          std::make_shared<CAMDEREncoder>(), recvHandler,
                          errHandler);

  int sendCount = 0;
  do {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    CAMWrapper cam(
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
  using namespace level::denm;

  const int msgsToSend = 100;
  bool recvFlags[msgsToSend] = {false};
  int errCount = 0;
  int recvMsgCount = 0;

  std::function<void(DENM *, bool *)> recvCallback =
      [&recvMsgCount, &recvFlags](DENM *msg, bool *tookOwnership) {
        *tookOwnership = true;
        DENMWrapper denm(msg);
        int index = denm->header.stationID;
        recvFlags[index] = true;
        recvMsgCount++;
      };
  auto recvHandler = std::make_shared<DENMRecvHandler>();
  recvHandler->registerCallback(recvCallback);

  std::function<void(const Exception &)> errCallback =
      [&errCount](const Exception &e) { errCount++; };
  auto errHandler = std::make_shared<ErrorHandler>();
  errHandler->registerCallback(errCallback);

  unsigned int port = 50000 + rand() % 10000;
  DENMNetworkInterface nal(std::make_shared<UDPSocket>(port, sys()),
                           std::make_shared<UDPSocket>(port, sys()),
                           std::make_shared<DENMDEREncoder>(), recvHandler,
                           errHandler);

  int sendCount = 0;
  do {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    DENMWrapper denm(
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