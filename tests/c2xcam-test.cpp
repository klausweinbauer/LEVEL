#include <gtest/gtest.h>
#include <c2xcam.h>
#include <sstream>
#include <c2xcommon.h>

TEST(CAM_Basic, Create_And_Delete_CAM) {
    int id = c2x::createCAM(1, 1);
    c2x::deleteCAM(id);
    ASSERT_GT(id, 0);
}

TEST(CAM_Basic, Create_CAM_With_The_Same_ID_Twice) {
    int ret1 = c2x::createCAM(1, 1);
    int ret2 = c2x::createCAM(1, 1);
    c2x::deleteCAM(1);
    ASSERT_EQ(ret1, 1);
    ASSERT_EQ(ret2, ERR_CAM_ALREADY_EXISTS);
}

TEST(CAM_Basic, Double_Delete_CAM) {
    int id = c2x::createCAM(1, 1);
    int ret1 = c2x::deleteCAM(id);
    int ret2 = c2x::deleteCAM(id);
    ASSERT_EQ(0, ret1);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, ret2);
}

TEST(CAM_Values, Set_And_Get_CAM_Header_Values) {
    int id = c2x::createCAM(1, 1);

    int protVers, msgId;
    c2x::setCAMHeader(id, 1, 2);
    c2x::getCAMHeader(id, &protVers, &msgId);

    c2x::deleteCAM(id);
    ASSERT_EQ(1, protVers);
    ASSERT_EQ(2, msgId);
}

TEST(CAM_Values, Get_CAM_Header_Values_With_Nullptr_Parameters) {
    int id = c2x::createCAM(1, 1);

    c2x::getCAMHeader(id, nullptr, nullptr);

    c2x::deleteCAM(id);
}

TEST(CAM_Values, Set_And_Get_CAM_GenerationDeltaTime_Value) {
    int id = c2x::createCAM(1, 1);

    int dt;
    c2x::setCAMGenerationDeltaTime(id, 4);
    c2x::getCAMGenerationDeltaTime(id, &dt);

    c2x::deleteCAM(id);
    ASSERT_EQ(4, dt);
}

TEST(CAM_Values, Get_CAM_GenerationDeltaTime_Value_With_Nullptr_Parameter) {
    int id = c2x::createCAM(1, 1);
    c2x::getCAMGenerationDeltaTime(id, nullptr);
    c2x::deleteCAM(id);
}

TEST(CAM_Values, Set_And_Reset_BitString) {
    int id = c2x::createCAM(1, HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE);
    uint8_t buffer1[4] = { 1, 2, 3, 4 };
    uint8_t buffer2[6] = {1, 2, 3, 4, 5, 6};
    uint8_t buffer3[2] = {1, 2};
    c2x::setCAMBasicVehicleContainerHighFrequencyAccelerationControl(
        id, buffer1, 4
    );
    c2x::setCAMBasicVehicleContainerHighFrequencyAccelerationControl(
        id, buffer2, 6
    );
    c2x::setCAMBasicVehicleContainerHighFrequencyAccelerationControl(
        id, buffer3, 2
    );
    c2x::deleteCAM(id);
}

TEST(CAM_Coding, Encode_CAM_Message) {
    int id = c2x::createCAM(1, 1);
    uint8_t buffer[4096];

    int ret = c2x::encodeCAM(id, buffer, 4096, nullptr);

    ASSERT_GT(ret, 0);

    c2x::deleteCAM(id);
}

TEST(CAM_Coding, Decode_CAM_And_Override_Message) {
    int id = c2x::createCAM(1, 1);
    c2x::setCAMHeader(id, 1, 2);

    uint8_t buffer[4096];    
    int size = c2x::encodeCAM(id, buffer, 4096, nullptr);
    int  newProtVers, newMsgId, newStatId;
    c2x::decodeCAM(&newStatId, buffer, size);
    c2x::getCAMHeader(newStatId, &newProtVers, &newMsgId);

    ASSERT_EQ(1, newProtVers);
    ASSERT_EQ(2, newMsgId);
    ASSERT_EQ(1, newStatId);

    c2x::deleteCAM(id);
}

TEST(CAM_Network, Start_And_Stop_Receiver) {
    int retStart1 = c2x::startCAMReceiver(1997);
    int retStop1 = c2x::stopCAMReceiver();
    int retStart2 = c2x::startCAMReceiver(1997);
    int retStop2 = c2x::stopCAMReceiver();
    
    ASSERT_EQ(0, retStart1);
    ASSERT_EQ(0, retStop1);
    ASSERT_EQ(0, retStart2);
    ASSERT_EQ(0, retStop2);
}

TEST(CAM_Coding, Decode_New_Message) {
    c2x::createCAM(1, 1);
    uint8_t buffer[4000];
    int size;
    c2x::encodeCAM(1, buffer, 4000, &size);
    c2x::deleteCAM(1);

    int id;
    int ret = c2x::decodeCAM(&id, buffer, size);
    int retDel = c2x::deleteCAM(1);
    ASSERT_EQ(1, id);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(0, retDel);
}

TEST(Error_Msg, Set_And_Get_Last_Error_Message) {
    std::stringstream ss;
    ss << "Hello World" << std::endl;
    c2x::setLastErrMsg(ss.str().c_str(), ss.str().size());

    char buffer[128];
    int actualSize = 0;
    c2x::getLastErrMsg(buffer, 128, &actualSize);
    ASSERT_STREQ(ss.str().c_str(), buffer);
}

TEST(CAM_HighFrequencyContainer, Check_HighFrequencyAccelerationControl_Errors)
{
    int msgNotFoundGet = c2x::getCAMBasicVehicleContainerHighFrequencyAccelerationControl(1, nullptr, 0, nullptr);
    int msgNotFoundSet = c2x::setCAMBasicVehicleContainerHighFrequencyAccelerationControl(1, nullptr, 0);
    EXPECT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    EXPECT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundSet);
}

TEST(CAM_HighFrequencyContainer, Set_And_Get_HighFrequencyAccelerationControl_Values)
{
    c2x::createCAM(1, HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE);
    uint8_t accelControl[] = { 1, 2, 3, 4 }, accelControl_out[8];
    int actualSize;
    int msgNotFoundSet = c2x::setCAMBasicVehicleContainerHighFrequencyAccelerationControl(1, accelControl, 4);
    int msgNotFoundGet = c2x::getCAMBasicVehicleContainerHighFrequencyAccelerationControl(1, accelControl_out, 8, &actualSize);
    EXPECT_EQ(accelControl[0], accelControl_out[0]);
    EXPECT_EQ(accelControl[1], accelControl_out[1]);
    EXPECT_EQ(accelControl[2], accelControl_out[2]);
    EXPECT_EQ(accelControl[3], accelControl_out[3]);
    EXPECT_EQ(actualSize, 4);
    c2x::deleteCAM(1);
}

TEST(CAM_RSUContainerHighFrequency, Check_RSUContainerHighFrequency_Errors)
{
    int msgNotFoundAdd = c2x::addCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 0, 0, 0, 0, 0, 0);
    int msgNotFoundGet = c2x::getCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 0, nullptr, 0);
    int msgNotFoundClr = c2x::clearCAMRSUContainerHighFrequencyProtectedCommunicationZones(1);
    
    c2x::createCAM(1, HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE);
    int wrongContainer = c2x::addCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 0, 0, 0, 0, 0, 0);
    c2x::deleteCAM(1);

    c2x::createCAM(1, HIGH_FREQ_CONTAINER_TYPE_RSU);
    int buffer[6];
    int errNull = c2x::getCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 0, buffer, 6);
    c2x::addCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 1, 2, 3, 4, 5, 6);
    int errNullBuffer = c2x::getCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 0, nullptr, 0);
    int indexOutOfRange = c2x::getCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 1, buffer, 6);
    int bufferToSmall = c2x::getCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 0, buffer, 5);
    c2x::deleteCAM(1);

    EXPECT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundAdd);
    EXPECT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundGet);
    EXPECT_EQ(ERR_MSG_NOT_FOUND, msgNotFoundClr);
    EXPECT_EQ(ERR_HIGH_FREQ_CONTAINER_TYPE, wrongContainer);
    EXPECT_EQ(ERR_NULL, errNullBuffer);
    EXPECT_EQ(ERR_NULL, errNull);
    EXPECT_EQ(ERR_INDEX_OUT_OF_RANGE, indexOutOfRange);
    EXPECT_EQ(ERR_BUFFER_OVERFLOW, bufferToSmall);
}

TEST(CAM_RSUContainerHighFrequency, RSUContainerHighFrequency_ProtectedCommunicationZone_Values)
{
    c2x::createCAM(1, HIGH_FREQ_CONTAINER_TYPE_RSU);
    c2x::addCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 1, 2, 3, 4, 5, 6);
    c2x::addCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 11, 12, 13, 14, 15, 16);
    int zone1[6], zone2[6], zone3[6], zonex[6];
    c2x::getCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 0, zone1, 6);
    c2x::getCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 1, zone2, 6);
    c2x::clearCAMRSUContainerHighFrequencyProtectedCommunicationZones(1);
    int errIndexOutOfRange = c2x::getCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 0, zonex, 6);
    c2x::addCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 21, 22, 23, 24, 25, 26);
    c2x::getCAMRSUContainerHighFrequencyProtectedCommunicationZone(1, 0, zone3, 6);
    c2x::deleteCAM(1);

    EXPECT_EQ(ERR_INDEX_OUT_OF_RANGE, errIndexOutOfRange);
    EXPECT_EQ(1, zone1[0]);
    EXPECT_EQ(11, zone2[0]);
    EXPECT_EQ(21, zone3[0]);
    EXPECT_EQ(2, zone1[1]);
    EXPECT_EQ(12, zone2[1]);
    EXPECT_EQ(22, zone3[1]);
    EXPECT_EQ(3, zone1[2]);
    EXPECT_EQ(13, zone2[2]);
    EXPECT_EQ(23, zone3[2]);
    EXPECT_EQ(4, zone1[3]);
    EXPECT_EQ(14, zone2[3]);
    EXPECT_EQ(24, zone3[3]);
    EXPECT_EQ(5, zone1[4]);
    EXPECT_EQ(15, zone2[4]);
    EXPECT_EQ(25, zone3[4]);
    EXPECT_EQ(6, zone1[5]);
    EXPECT_EQ(16, zone2[5]);
    EXPECT_EQ(26, zone3[5]);
}

TEST(CAM_SpecialVehicleContainer, Error_Messages) 
{
    int errMsgNotFound = c2x::setCAMPublicTransportContainer(1, 0, 0, nullptr, 0);
    c2x::createCAM(1, HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE);
    int errNull = c2x::getCAMPublicTransportContainer(1, nullptr, nullptr, nullptr, 0);
    c2x::setCAMPublicTransportContainer(1, 0, 0, nullptr, 0);
    int errType = c2x::setCAMDangerousGoodsContainer(1, 0);
    char buffer[255];
    int len = 0;
    c2x::getLastErrMsg(buffer, 255, &len);
    c2x::deleteCAM(1);

    ASSERT_EQ(ERR_MSG_NOT_FOUND, errMsgNotFound);
    ASSERT_EQ(ERR_NULL, errNull); 
    ASSERT_EQ(ERR_SPECIAL_VEHICLE_CONTAINER_TYPE, errType);
    ASSERT_STREQ(buffer, "Wrong type of SpecialVehicleContainer. Container is of type 'PublicTransportContainer' but type 'DangerousGoodsContainer' is needed.\n");
}

TEST(CAM_SpecialVehicleContainer, Set_And_Get_PublicTransportContainer)
{
    c2x::createCAM(1, HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE);
    c2x::deleteCAM(1);
}