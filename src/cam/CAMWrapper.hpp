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
  static DEREncoder _copyEncoder;

public:
  CAMWrapper();
  CAMWrapper(CAM *cam);
  CAMWrapper(int stationId);
  CAMWrapper(int stationId, HighFrequencyContainer_PR type);

  ~CAMWrapper();

  CAMWrapper(const CAMWrapper &other);
  CAMWrapper &operator=(CAMWrapper other);

  CAM *operator->();
  CAM &operator*();

  CAM *get();

  /**
   * @brief Set LowFrequencyContainer.
   *
   * @throw Exception if other container is set.
   *
   * @param type Container type.
   * @return LowFrequencyContainer* LowFrequencyContainer or nullptr if set to
   * NOTHING.
   */
  LowFrequencyContainer *setLFC(LowFrequencyContainer_PR type);

  /**
   * @brief Set HighFrequencyContainer.
   *
   * @throw Exception if other container is set.
   *
   * @param type Container type.
   * @return HighFrequencyContainer* HighFrequencyContainer.
   */
  HighFrequencyContainer *setHFC(HighFrequencyContainer_PR type);

  /**
   * @brief Set SpecialVehicleContainer.
   *
   * @throw Exception if other container is set.
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