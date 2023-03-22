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

/**
 * @file CAMWrapper.hpp
 * @author Klaus Weinbauer
 * @brief Memory safe wrapper for CAMs.
 * @version 0.1
 * @date 2022-07-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <CAM.h>
#include <CAMEncoder.hpp>
#include <Types.hpp>
#include <algorithm>
#include <level.h>

#define CAM_MESSAGE_ID 2

namespace level {
namespace cam {

/**
 * @brief Memory safe wrapper for CAMs.
 *
 */
class CAMWrapper {

private:
  CAM *_cam;
  static CAMDEREncoder _copyEncoder;

  void initHFC();

public:
  CAMWrapper();
  CAMWrapper(CAM *cam);
  CAMWrapper(unsigned int stationId);
  CAMWrapper(unsigned int stationId, HighFrequencyContainer_PR type);

  ~CAMWrapper();

  CAMWrapper(const CAMWrapper &other);
  CAMWrapper &operator=(CAMWrapper other);

  CAM *operator->() const;
  CAM &operator*() const;

  CAM *get() const;

  /**
   * @brief Set LowFrequencyContainer.
   *
   * @param type Container type.
   * @return LowFrequencyContainer* LowFrequencyContainer or nullptr if set to
   * NOTHING.
   */
  LowFrequencyContainer *setLFC(LowFrequencyContainer_PR type);

  /**
   * @brief Set HighFrequencyContainer.
   *
   * @param type Container type.
   * @return HighFrequencyContainer* HighFrequencyContainer.
   */
  HighFrequencyContainer *setHFC(HighFrequencyContainer_PR type);

  /**
   * @brief Set SpecialVehicleContainer.
   *
   * @param type Container type.
   * @return SpecialVehicleContainer* SpecialVehicleContainer or nullptr if set
   * to NOTHING.
   */
  SpecialVehicleContainer *setSVC(SpecialVehicleContainer_PR type);

  /**
   * @brief Get LowFrequencyContainer.
   *
   * @return LowFrequencyContainer* LowFrequencyContainer or nullptr if not set.
   */
  LowFrequencyContainer *getLFC() const;

  /**
   * @brief Get HighFrequencyContainer.
   *
   * @return HighFrequencyContainer* HighFrequencyContainer.
   */
  HighFrequencyContainer *getHFC() const;

  /**
   * @brief Get SpecialVehicleContainer.
   *
   * @return SpecialVehicleContainer* SpecialVehicleContainer or nullptr if not
   * set.
   */
  SpecialVehicleContainer *getSVC() const;

  /**
   * @brief Delete LowFrequencyContainer.
   *
   */
  void clearLFC();

  /**
   * @brief Clear HighFrequencyContainer.
   *
   */
  void clearHFC();

  /**
   * @brief Delete SpecialVehicleContainer.
   *
   */
  void clearSVC();

  /**
   * @brief Fill the basic container data struct with the current values.
   *
   * @param container Container to fill with data.
   * @return int 0 on success or error code.
   */
  int getBasicContainer(CAMBasicContainer_t *container) const;

  /**
   * @brief Fill the basic vehicle container high frequency data struct with the
   * current values.
   *
   * @param container Container to fill with data.
   * @return int 0 on success or error code.
   */
  int getBasicVehicleContainerHighFrequency(
      CAMBasicVehicleContainerHF_t *container) const;

  friend void swap(CAMWrapper &first, CAMWrapper &second) {
    using std::swap;
    swap(first._cam, second._cam);
  }
};

} // namespace cam

#define CAM_TYPE_NAME "CAM"

template <> struct TypeName<CAM> {
  static const char *get() { return CAM_TYPE_NAME; }
};

template <> struct TypeName<cam::CAMWrapper> {
  static const char *get() { return CAM_TYPE_NAME; }
};

} // namespace level