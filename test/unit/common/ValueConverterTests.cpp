#include <ValueConverter.hpp>
#include <gtest/gtest.h>
#include <level.h>

using namespace level;

TEST(ValueConverter, SetHeading) {
  ValueConverter converter;
  float siValue = 90;
  int itsExpected = 900;
  float siExpected = siValue;
  int itsValue = converter.siToITSHeading(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSIHeading(itsValue));
}

TEST(ValueConverter, SetHeadingWrapUpper) {
  ValueConverter converter;
  float siValue = 540;
  int itsExpected = 1800;
  float siExpected = 180;
  int itsValue = converter.siToITSHeading(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSIHeading(itsValue));
}

TEST(ValueConverter, SetHeadingWrapLower) {
  ValueConverter converter;
  float siValue = -90;
  int itsExpected = 2700;
  float siExpected = 270;
  int itsValue = converter.siToITSHeading(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSIHeading(itsValue));
}

TEST(ValueConverter, SetHeadingWrapMultipleLower) {
  ValueConverter converter;
  float siValue = -780;
  int itsExpected = 3000;
  float siExpected = 300;
  int itsValue = converter.siToITSHeading(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSIHeading(itsValue));
}

TEST(ValueConverter, SetSpeed) {
  ValueConverter converter;
  float siValue = 100;
  int itsExpected = 10000;
  float siExpected = siValue;
  int itsValue = converter.siToITSSpeed(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSISpeed(itsValue));
}

TEST(ValueConverter, SetSpeedUpperBound) {
  ValueConverter converter;
  float siValue = 163.83;
  int itsExpected = 16382;
  float siExpected = 163.82;
  int itsValue = converter.siToITSSpeed(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSISpeed(itsValue));
}

TEST(ValueConverter, SetSpeedLowerBound) {
  ValueConverter converter;
  float siValue = -1;
  int itsExpected = 0;
  float siExpected = 0;
  int itsValue = converter.siToITSSpeed(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSISpeed(itsValue));
}

/*TEST(ValueConverter, SetDriveDirection) {
  ValueConverter converter;
  int itsValue = converter.siToITSDriveDirection(DirveDirection_Backward);
  ASSERT_EQ((int)DirveDirection_Backward, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSIDriveDirection(itsValue));
}

TEST(ValueConverter, SetInvalidDriveDirection) {
  ValueConverter converter;
  int itsValue = converter.siToITSDriveDirection((DriveDirectionType)100);
  ASSERT_EQ((int)DriveDirection_Unavailable, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSIDriveDirection(itsValue));
}*/

TEST(ValueConverter, SetLongitudinalAcceleration) {
  ValueConverter converter;
  float siValue = 10;
  int itsExpected = 100;
  float siExpected = siValue;
  int itsValue = converter.siToITSLongitudinalAcceleration(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSILongitudinalAcceleration(itsValue));
}

TEST(ValueConverter, SetAccelerationUpperBound) {
  ValueConverter converter;
  float siValue = 100;
  int itsExpected = 160;
  float siExpected = 16;
  int itsValue = converter.siToITSLongitudinalAcceleration(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSILongitudinalAcceleration(itsValue));
}

TEST(ValueConverter, SetAccelerationLowerBound) {
  ValueConverter converter;
  float siValue = -100;
  int itsExpected = -160;
  float siExpected = -16;
  int itsValue = converter.siToITSLongitudinalAcceleration(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSILongitudinalAcceleration(itsValue));
}

TEST(ValueConverter, SetCurvature) {
  ValueConverter converter;
  float siValue = 10;
  int itsExpected = 1000;
  float siExpected = siValue;
  int itsValue = converter.siToITSCurvature(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSICurvature(itsValue));
}

TEST(ValueConverter, SetCurvatureUpperBound) {
  ValueConverter converter;
  float siValue = 1;
  int itsExpected = 1022;
  float siExpected = 10000.0 / 1022;
  int itsValue = converter.siToITSCurvature(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSICurvature(itsValue));
}

TEST(ValueConverter, SetCurvatureLowerBound) {
  ValueConverter converter;
  float siValue = -1;
  int itsExpected = -1023;
  float siExpected = 10000.0 / -1023;
  int itsValue = converter.siToITSCurvature(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSICurvature(itsValue));
}

TEST(ValueConverter, SetCurvatureStraight) {
  ValueConverter converter;
  float siValue = 0;
  int itsExpected = 0;
  float siExpected = 0;
  int itsValue = converter.siToITSCurvature(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSICurvature(itsValue));
}

TEST(ValueConverter, SetYawRate) {
  ValueConverter converter;
  float siValue = 10;
  int itsExpected = 1000;
  float siExpected = 10;
  int itsValue = converter.siToITSYawRate(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSIYawRate(itsValue));
}

TEST(ValueConverter, SetYawRateUpperBound) {
  ValueConverter converter;
  float siValue = 327.67;
  int itsExpected = 32766;
  float siExpected = 327.66;
  int itsValue = converter.siToITSYawRate(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSIYawRate(itsValue));
}

TEST(ValueConverter, SetYawRateLowerBound) {
  ValueConverter converter;
  float siValue = -327.67;
  int itsExpected = -32766;
  float siExpected = -327.66;
  int itsValue = converter.siToITSYawRate(siValue);
  ASSERT_EQ(itsExpected, itsValue);
  ASSERT_EQ(siExpected, converter.itsToSIYawRate(itsValue));
}