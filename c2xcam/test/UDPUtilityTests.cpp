#include <NetworkException.hpp>
#include <PacketReceiver.hpp>
#include <UDPSocket.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace c2x;

TEST(UDPUtilityTests, Test_Open_Multiple_Sockets) {

  UDPSocket socket1;
  UDPSocket socket2;

  ASSERT_NE(&socket1, &socket2);
}

TEST(UDPUtilityTests, Test_Correct_Bound_Socket_Management) {

  bool exceptionRaised = false;

  try {
    {
      UDPSocket socket;
      socket.bindSocket(5999);
    }
    {
      UDPSocket socket;
      socket.bindSocket(5999);
    }
  } catch (const std::exception &e) {
    exceptionRaised = true;
  }

  ASSERT_FALSE(exceptionRaised);
}

TEST(UDPUtilityTests, Test_Exception_On_Multiple_Binds) {

  int errCode = 0;
  UDPSocket socket;
  socket.bindSocket(5999);

  try {
    socket.bindSocket(5999);
  } catch (const NetworkException &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR, errCode);
}

TEST(UDPUtilityTests, Test_Packet_Receiver_Instantiation_And_Cleanup) {

  bool exceptionRaised = false;

  try {
    { PacketReceiver receiver(5999); }
    { PacketReceiver receiver(5999); }
  } catch (const std::exception &e) {
    exceptionRaised = true;
  }

  ASSERT_FALSE(exceptionRaised);
}

static int testSendAndReceiveDataLen = 0;
void recvPacket(char *buffer, int len) { testSendAndReceiveDataLen = len; }

TEST(UDPUtilityTests, Test_Send_And_Receive_Data) {

  unsigned short port = 5999;
  std::string msg = "Hello Receiver!";
  UDPSocket sender;
  PacketReceiver receiver(port);
  receiver.recvPacketCallback = recvPacket;
  sender.sendTo(port, msg.c_str(), msg.length() + 1);

  while (!testSendAndReceiveDataLen) {
    usleep(1);
  }

  ASSERT_EQ(msg.length() + 1, testSendAndReceiveDataLen);
}