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
  CAMWrapper(int stationId);
  CAMWrapper(int stationId, HighFrequencyContainer_PR type);

  ~CAMWrapper();

  CAMWrapper(const CAMWrapper &other);
  CAMWrapper &operator=(CAMWrapper other);

  CAM *operator->();

  void setLFC(LowFrequencyContainer_PR type);

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