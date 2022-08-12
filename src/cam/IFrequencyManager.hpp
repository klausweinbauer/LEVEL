#pragma once

#include <CAMWrapper.hpp>

namespace level::cam {

class IFrequencyManager {
public:
  virtual ~IFrequencyManager() {}

  /**
   * @brief The interval in which CAM generation is checked. This shall be
   * equal to or less than GenCAMMin and at most 1000 ms.
   *
   * @return int Interval to check CAM generation in ms.
   */
  virtual int getTCheckCAMGen() = 0;

  /**
   * @brief The dynamic interval between two CAM generations. This may change
   * depending on available network bandwidth or CAM value changes.
   *
   * @return int Interval between to CAM generations in ms.
   */
  virtual int getTCAMGen() = 0;

  /**
   * @brief Checks if a new CAM shall be generated.
   *
   * @param cam The current local CAM state.
   * @return true A new CAM shall be generated.
   * @return false No CAM generation required.
   */
  virtual bool generateCAM(CAMWrapper &cam) = 0;

  /**
   * @brief Informs the CA Basic Service if the low frequency container should
   * be included in this CAM generation.
   *
   * @return true LowFrequencyContainer is included.
   * @return false LowFrequencyContainer is not included.
   */
  virtual bool includeLFC() = 0;

  /**
   * @brief This method gets called by the CA Basic Service if CAM generation
   * was successful so the Frequency Manager can update its state.
   *
   * @param cam The newly generated CAM.
   */
  virtual void notifyCAMGeneration(CAMWrapper &cam) = 0;
};

} // namespace level::cam