#include <NetworkException.hpp>
#include <PacketReceiver.hpp>
#include <UDPSocket.hpp>
#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace level;

static int port = 40200;

std::shared_ptr<ISocket> socket(int port = 5999) {
  return std::shared_ptr<ISocket>(new UDPSocket(port));
}

TEST(Common_PacketReceiver, Test_Packet_Receiver_Instantiation_And_Cleanup) {

  bool exceptionRaised = false;

  try {
    { PacketReceiver receiver(socket(port)); }
    { PacketReceiver receiver(socket(port)); }
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

  port++;
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