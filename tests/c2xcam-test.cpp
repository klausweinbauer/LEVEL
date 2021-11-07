#include <gtest/gtest.h>
#include <c2xcam.h>

TEST(Basic, Create_And_Delete_CAM) {
    int id = c2x::createCAM(1);
    c2x::deleteCAM(id);
    ASSERT_GT(id, 0);
}

TEST(Basic, Create_CAM_With_The_Same_ID_Twice) {
    int ret1 = c2x::createCAM(1);
    int ret2 = c2x::createCAM(1);
    c2x::deleteCAM(1);
    ASSERT_EQ(ret1, 1);
    ASSERT_EQ(ret2, ERR_CAM_ALREADY_EXISTS);
}

TEST(Basic, Double_Delete_CAM) {
    int id = c2x::createCAM(1);
    int ret1 = c2x::deleteCAM(id);
    int ret2 = c2x::deleteCAM(id);
    ASSERT_EQ(0, ret1);
    ASSERT_EQ(ERR_MSG_NOT_FOUND, ret2);
}

TEST(CAM_Values, Set_And_Get_CAM_Header_Values) {
    int id = c2x::createCAM(1);

    int protVers, msgId;
    c2x::setCAMHeader(id, 1, 2);
    c2x::getCAMHeader(id, &protVers, &msgId);

    c2x::deleteCAM(id);
    ASSERT_EQ(1, protVers);
    ASSERT_EQ(2, msgId);
}

TEST(CAM_Values, Get_CAM_Header_Values_With_Nullptr_Parameters) {
    int id = c2x::createCAM(1);

    c2x::getCAMHeader(id, nullptr, nullptr);

    c2x::deleteCAM(id);
}

TEST(CAM_Values, Set_And_Get_CAM_GenerationDeltaTime_Value) {
    int id = c2x::createCAM(1);

    int dt;
    c2x::setCAMGenerationDeltaTime(id, 4);
    c2x::getCAMGenerationDeltaTime(id, &dt);

    c2x::deleteCAM(id);
    ASSERT_EQ(4, dt);
}

TEST(CAM_Values, Get_CAM_GenerationDeltaTime_Value_With_Nullptr_Parameter) {
    int id = c2x::createCAM(1);
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
    int id = c2x::createCAM(1);
    uint8_t buffer[4096];

    int ret = c2x::encodeCAM(id, buffer, 4096);

    ASSERT_GT(ret, 0);

    c2x::deleteCAM(id);
}

TEST(CAM_Coding, Decode_CAM_And_Override_Message) {
    int id = c2x::createCAM(1);
    c2x::setCAMHeader(id, 1, 2);

    uint8_t buffer[4096];    
    int size = c2x::encodeCAM(id, buffer, 4096);
    int  newProtVers, newMsgId, newStatId;
    c2x::decodeCAM(&newStatId, buffer, size);
    c2x::getCAMHeader(newStatId, &newProtVers, &newMsgId);

    ASSERT_EQ(1, newProtVers);
    ASSERT_EQ(2, newMsgId);
    ASSERT_EQ(1, newStatId);

    c2x::deleteCAM(id);
}