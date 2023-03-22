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