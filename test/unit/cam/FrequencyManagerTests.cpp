#include <IFrequencyManager.hpp>
#include <Mocks.hpp>
#include <VehicleFrequencyManager.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::NiceMock;

namespace level::FrequencyManagerTest {

std::shared_ptr<cam::IFrequencyManager> getVehicleFM() {
  return std::make_shared<cam::VehicleFrequencyManager>();
}

std::shared_ptr<cam::IFrequencyManager> getRSUFM() {}

} // namespace level::FrequencyManagerTest

using namespace level::FrequencyManagerTest;
using namespace level::cam;