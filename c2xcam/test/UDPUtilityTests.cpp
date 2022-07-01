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

  {
    UDPSocket socket;
    socket.bindSocket(5999);
  }
  {
    UDPSocket socket;
    socket.bindSocket(5999);
  }

  ASSERT_TRUE(true); // If we get here, everything is fine
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