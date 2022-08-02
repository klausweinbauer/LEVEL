#include <gtest/gtest.h>
#include <level.h>

using namespace level;

TEST(VDPTests, SetHeading) {
  VDP_t vdp;
  float value = 90;
  int expectedValue = 900;
  vdp.setHeading(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().heading);
}

TEST(VDPTests, SetHeadingWrapUpper) {
  VDP_t vdp;
  float value = 540;
  int expectedValue = 1800;
  vdp.setHeading(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().heading);
}

TEST(VDPTests, SetHeadingWrapLower) {
  VDP_t vdp;
  float value = -90;
  int expectedValue = 2700;
  vdp.setHeading(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().heading);
}

TEST(VDPTests, SetHeadingWrapMultipleLower) {
  VDP_t vdp;
  float value = -780;
  int expectedValue = 3000;
  vdp.setHeading(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().heading);
}

TEST(VDPTests, SetSpeed) {
  VDP_t vdp;
  float value = 100;
  int expectedValue = 10000;
  vdp.setSpeed(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().speed);
}

TEST(VDPTests, SetSpeedUpperBound) {
  VDP_t vdp;
  float value = 163.83;
  int expectedValue = 16382;
  vdp.setSpeed(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().speed);
}

TEST(VDPTests, SetSpeedLowerBound) {
  VDP_t vdp;
  float value = -1;
  int expectedValue = 0;
  vdp.setSpeed(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().speed);
}

TEST(VDPTests, SetDriveDirection) {
  VDP_t vdp;
  vdp.setDriveDirection(DirveDirection_Backward);
  ASSERT_EQ((int)DirveDirection_Backward, vdp.getCAMDataUnit().driveDirection);
}

TEST(VDPTests, SetInvalidDriveDirection) {
  VDP_t vdp;
  vdp.setDriveDirection((DriveDirectionType)100);
  ASSERT_EQ((int)DriveDirection_Unavailable,
            vdp.getCAMDataUnit().driveDirection);
}

TEST(VDPTests, SetAcceleration) {
  VDP_t vdp;
  float value = 10;
  int expectedValue = 100;
  vdp.setAcceleration(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().acceleration);
}

TEST(VDPTests, SetAccelerationUpperBound) {
  VDP_t vdp;
  float value = 100;
  int expectedValue = 160;
  vdp.setAcceleration(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().acceleration);
}

TEST(VDPTests, SetAccelerationLowerBound) {
  VDP_t vdp;
  float value = -100;
  int expectedValue = -160;
  vdp.setAcceleration(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().acceleration);
}

TEST(VDPTests, SetCurvature) {
  VDP_t vdp;
  float radius = 10;
  int expectedValue = 1000;
  vdp.setCurvature(radius);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().curvature);
}

TEST(VDPTests, SetCurvatureUpperBound) {
  VDP_t vdp;
  float radius = 1;
  int expectedValue = 1022;
  vdp.setCurvature(radius);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().curvature);
}

TEST(VDPTests, SetCurvatureLowerBound) {
  VDP_t vdp;
  float radius = -1;
  int expectedValue = -1023;
  vdp.setCurvature(radius);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().curvature);
}

TEST(VDPTests, SetCurvatureStraight) {
  VDP_t vdp;
  float radius = 0;
  int expectedValue = 0;
  vdp.setCurvature(radius);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().curvature);
}

TEST(VDPTests, SetYawRate) {
  VDP_t vdp;
  float value = 10;
  int expectedValue = 1000;
  vdp.setYawRate(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().yawRate);
}

TEST(VDPTests, SetYawRateUpperBound) {
  VDP_t vdp;
  float value = 327.67;
  int expectedValue = 32766;
  vdp.setYawRate(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().yawRate);
}

TEST(VDPTests, SetYawRateLowerBound) {
  VDP_t vdp;
  float value = -327.67;
  int expectedValue = -32766;
  vdp.setYawRate(value);
  ASSERT_EQ(expectedValue, vdp.getCAMDataUnit().yawRate);
}