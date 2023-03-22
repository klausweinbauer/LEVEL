// Copyright 2022 Klaus Weinbauer
//
// This file is part of LEVEL.
//
// LEVEL is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// LEVEL is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LEVEL. If not, see <https://www.gnu.org/licenses/>.

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