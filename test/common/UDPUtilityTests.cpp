#include <NetworkException.hpp>
#include <PacketReceiver.hpp>
#include <UDPSocket.hpp>
#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace level;

std::shared_ptr<ISocket> socket(int port = 5999) {
  return std::shared_ptr<ISocket>(new UDPSocket(port));
}

TEST(Common_UDPSocket, Test_Open_Multiple_Sockets) {

  UDPSocket socket1(5999);
  UDPSocket socket2(5999);

  ASSERT_NE(&socket1, &socket2);
}

TEST(Common_UDPSocket, Test_Correct_Bound_Socket_Management) {

  bool exceptionRaised = false;

  try {
    {
      UDPSocket socket(5999);
      socket.bindSocket();
    }
    {
      UDPSocket socket(5999);
      socket.bindSocket();
    }
  } catch (const std::exception &) {
    exceptionRaised = true;
  }

  ASSERT_FALSE(exceptionRaised);
}

TEST(Common_UDPSocket, Test_Exception_On_Multiple_Binds) {

  int errCode = 0;
  UDPSocket socket(5999);
  socket.bindSocket();

  try {
    socket.bindSocket();
  } catch (const NetworkException &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR, errCode);
}

TEST(Common_UDPSocket, Test_Send_Null_Ptr) {

  UDPSocket socket(5999);

  socket.send(nullptr, 0);
}

TEST(Common_UDPSocket, Test_Send_Failed) {

  int errCode = 0;
  UDPSocket socket(5999);

  try {
    socket.send(nullptr, 0, 99);
  } catch (const NetworkException &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR, errCode);
}

TEST(Common_UDPSocket, Test_Receive_Failed) {

  int errCode = 0;
  UDPSocket socket(5999);

  try {
    socket.recv(nullptr, 0, 99);
  } catch (const NetworkException &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR, errCode);
}

TEST(Common_PacketReceiver, Test_Packet_Receiver_Instantiation_And_Cleanup) {

  bool exceptionRaised = false;

  try {
    { PacketReceiver receiver(socket(5999)); }
    { PacketReceiver receiver(socket(5999)); }
  } catch (const std::exception &e) {
    exceptionRaised = true;
  }

  ASSERT_FALSE(exceptionRaised);
}

static int testSendAndReceiveDataLen = 0;
void recvPacket(const char *buffer, int len) {
  testSendAndReceiveDataLen = len;
}

TEST(Common_PacketReceiver, Test_Send_And_Receive_Data) {

  unsigned short port = 5999;
  std::string msg = "Hello Receiver!";
  UDPSocket sender(port);
  PacketReceiver receiver(socket(port));
  receiver.recvPacketCallback = recvPacket;
  receiver.start();
  sender.send(msg.c_str(), msg.length() + 1);

  while (!testSendAndReceiveDataLen) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  ASSERT_EQ(msg.length() + 1, testSendAndReceiveDataLen);
}