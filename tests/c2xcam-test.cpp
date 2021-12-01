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
}