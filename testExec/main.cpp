#include <c2xcam.h>
#include <c2xdenm.h>
#include <iostream>
#include <sstream>

#ifndef WIN32
#include <unistd.h>
#endif

int main(int argc, char **argv) {
  // CAM Sizes
  /*int bufferLen = 20000;
  uint8_t buffer[bufferLen];
  int actualSize;
  uint8_t extLight[1] = {0};

  int id = c2x::createCAM(1);
  c2x::defineCAMHighFrequencyContainer(id, 1);
  c2x::setCAMBasicVehicleContainerLowFrequency(id, 0, extLight, 1);
  int retVal = c2x::encodeCAM(id, buffer, bufferLen, &actualSize, 0);

  char msgBuffer[200];
  int msgLen;
  c2x::getLastErrMsg(msgBuffer, 200, &msgLen);
  std::cout << msgBuffer << std::endl;

  std::cout << "(" << retVal << ") Encoded Size: " << actualSize << std::endl;*/

  // DENM Sizes
  int bufferLen = 20000;
  uint8_t buffer[bufferLen];
  int actualSize;
  int id = 1, seqNr = 1;

  c2x::createDENM(id, seqNr);
  c2x::setDENMSituationContainer(id, seqNr, 0, 0, 0);
  c2x::setDENMLocationContainerSpeed(id, seqNr, 0, 0);
  c2x::setDENMLocationContainerHeading(id, seqNr, 0, 0);
  c2x::setDENMLocationContainerRoadType(id, seqNr, 0);
  int retVal = c2x::encodeDENM(id, seqNr, buffer, bufferLen, &actualSize, 0);

  // char msgBuffer[200];
  // int msgLen;
  // c2x::getLastErrMsg(msgBuffer, 200, &msgLen);
  // std::cout << msgBuffer << std::endl;

  std::cout << "(" << retVal << ") Encoded Size: " << actualSize << std::endl;
}