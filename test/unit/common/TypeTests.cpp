#include <CAMWrapper.hpp>
#include <DENMWrapper.hpp>
#include <Types.hpp>
#include <gtest/gtest.h>

using namespace level;

TEST(Type, GetDefaultTypeName) {
  ASSERT_EQ(typeid(int).name(), TypeName<int>::get());
}

TEST(Type, GetDENMTypeName) {
  ASSERT_EQ(std::string(DENM_TYPE_NAME), TypeName<DENM>::get());
}

TEST(Type, GetDENMWrapperTypeName) {
  ASSERT_EQ(std::string(DENM_TYPE_NAME), TypeName<denm::DENMWrapper>::get());
}

TEST(Type, GetCAMTypeName) {
  ASSERT_EQ(std::string(CAM_TYPE_NAME), TypeName<CAM>::get());
}

TEST(Type, GetCAMWrapperTypeName) {
  ASSERT_EQ(std::string(CAM_TYPE_NAME), TypeName<cam::CAMWrapper>::get());
}