#include <gtest/gtest.h>
#include <level.h>

using namespace level;

TEST(API, SetAndGetCABasicServiceConfiguration) {
  CABasicServiceConfig_t expectedConfig;
  expectedConfig.stationID = rand();
  ASSERT_EQ(0, configureCABasicService(expectedConfig));
  auto config = getCABasicServiceConfig();
  ASSERT_EQ(expectedConfig.stationID, config.stationID);
}