#include <gtest/gtest.h>
#include <level.h>

using namespace level;

TEST(API, SetAndGetCABasicServiceConfiguration) {
  CABasicServiceConfig_t expectedConfig = getCABasicServiceConfig();
  expectedConfig.stationID = rand();
  expectedConfig.stationType = StationType_Trailer;
  ASSERT_EQ(0, configureCABasicService(expectedConfig));
  auto config = getCABasicServiceConfig();
  ASSERT_EQ(expectedConfig.stationID, config.stationID);
  ASSERT_EQ(expectedConfig.stationType, config.stationType);
}

TEST(API, ConfigurationCABasicServiceWithStationTypeUnknown) {
  CABasicServiceConfig_t expectedConfig = getCABasicServiceConfig();
  expectedConfig.stationType = StationType_Unknown;
  ASSERT_EQ(0, configureCABasicService(expectedConfig));
  auto config = getCABasicServiceConfig();
  ASSERT_EQ(expectedConfig.stationType, config.stationType);
  config.stationType = StationType_Trailer;
  configureCABasicService(config);
}

TEST(API, GetCAMBasicContainer) {}

TEST(API, GetCAMBasicVehicleContainerHighFrequency) {}