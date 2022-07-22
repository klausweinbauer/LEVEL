#include <NetworkException.hpp>
#include <PacketReceiver.hpp>
#include <UDPSocket.hpp>
#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace level;

static int port = 40100;

TEST(Common_UDPSocket, Test_Open_Multiple_Sockets) {

  UDPSocket socket1(port);
  UDPSocket socket2(port);

  ASSERT_NE(&socket1, &socket2);
}

TEST(Common_UDPSocket, Test_Correct_Bound_Socket_Management) {

  bool exceptionRaised = false;
  port++;

  try {
    {
      UDPSocket socket(port);
      socket.bindSocket();
    }
    {
      UDPSocket socket(port);
      socket.bindSocket();
    }
  } catch (const std::exception &) {
    exceptionRaised = true;
  }

  ASSERT_FALSE(exceptionRaised);
}

TEST(Common_UDPSocket, Test_Exception_On_Multiple_Binds) {

  int errCode = 0;
  port++;
  UDPSocket socket(port);
  socket.bindSocket();

  try {
    socket.bindSocket();
  } catch (const NetworkException &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR, errCode);
}

TEST(Common_UDPSocket, Test_Send_Null_Ptr) {

  port++;
  UDPSocket socket(port);

  socket.send(nullptr, 0);
}

TEST(Common_UDPSocket, Test_Send_Failed) {

  int errCode = 0;
  port++;
  UDPSocket socket(port);

  try {
    socket.send(nullptr, 0, 99);
  } catch (const NetworkException &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR, errCode);
}

TEST(Common_UDPSocket, Test_Receive_Failed) {

  int errCode = 0;
  port++;
  UDPSocket socket(port);

  try {
    socket.recv(nullptr, 0, 99);
  } catch (const NetworkException &e) {
    errCode = e.getErrCode();
  }

  ASSERT_EQ(ERR, errCode);
}