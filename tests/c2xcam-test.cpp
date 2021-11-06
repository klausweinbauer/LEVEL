#include <gtest/gtest.h>
#include <c2xcam.h>

TEST(Basic, Create_And_Delete_CAM) {
    int id = c2x::createCAM();
    c2x::deleteCAM(id);
    ASSERT_GT(id, 0);
}

TEST(CAM_Values, Set_And_Get_CAM_Header_Values) {
    int id = c2x::createCAM();

    int protVers, msgId, statId;
    c2x::setCAMHeader(id, 1, 2, 3);
    c2x::getCAMHeader(id, &protVers, &msgId, &statId);

    c2x::deleteCAM(id);
    ASSERT_EQ(1, protVers);
    ASSERT_EQ(2, msgId);
    ASSERT_EQ(3, statId);
}

TEST(CAM_Values, Get_CAM_Header_Values_With_Nullptr_Parameters) {
    int id = c2x::createCAM();

    c2x::getCAMHeader(id, nullptr, nullptr, nullptr);

    c2x::deleteCAM(id);
}

TEST(CAM_Values, Set_And_Get_CAM_GenerationDeltaTime_Value) {
    int id = c2x::createCAM();

    int dt;
    c2x::setCAMGenerationDeltaTime(id, 4);
    c2x::getCAMGenerationDeltaTime(id, &dt);

    c2x::deleteCAM(id);
    ASSERT_EQ(4, dt);
}

TEST(CAM_Values, Get_CAM_GenerationDeltaTime_Value_With_Nullptr_Parameter) {
    int id = c2x::createCAM();

    c2x::getCAMGenerationDeltaTime(id, nullptr);

    c2x::deleteCAM(id);
}

TEST(CAM_Values, Set_And_Reset_BitString) {
    int id = c2x::createCAM(HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE);
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