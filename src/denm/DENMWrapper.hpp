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
 * @file DENMWrapper.hpp
 * @author Klaus Weinbauer
 * @brief Memory safe wrapper for DENMs.
 * @version 0.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <DENM.h>
#include <DENMEncoder.hpp>
#include <Types.hpp>
#include <algorithm>
#include <level.h>

namespace level {
namespace denm {

/**
 * @brief Memory safe wrapper for DENMs.
 *
 */
class DENMWrapper {

private:
  DENM *_denm;
  static DENMDEREncoder _copyEncoder;

public:
  DENMWrapper();
  DENMWrapper(DENM *denm);
  DENMWrapper(int stationId, int sequenceNumber);
  DENMWrapper(ActionId_t actionId);

  ~DENMWrapper();

  DENMWrapper(const DENMWrapper &other);
  DENMWrapper &operator=(DENMWrapper other);

  DENM *operator->() const;
  DENM &operator*() const;

  DENM *get();

  DENMState getState() const;

  /**
   * @brief Get ManagementContainer.
   *
   * @return ManagementContainer* ManagementContainer.
   */
  ManagementContainer *getMC() const;

  /**
   * @brief Get SituationContainer.
   *
   * @return SituationContainer* SituationContainer or nullptr if not set.
   */
  SituationContainer *getSC() const;

  /**
   * @brief Get LocationContainer.
   *
   * @return LocationContainer* LocationContainer or nullptr if not set.
   */
  LocationContainer *getLC() const;

  /**
   * @brief Get AlacarteContainer.
   *
   * @return AlacarteContainer* AlacarteContainer or nullptr if not set.
   */
  AlacarteContainer *getAC() const;

  /**
   * @brief Set SituationContainer.
   *
   * @throw Exception if container already set.
   *
   * @return SituationContainer* SituationContainer.
   */
  SituationContainer *setSC();

  /**
   * @brief Set LocationContainer.
   *
   * @throw Exception if container already set.
   *
   * @return LocationContainer* LocationContainer.
   */
  LocationContainer *setLC();

  /**
   * @brief Set AlacarteContainer.
   *
   * @throw Exception if container already set.
   *
   * @return AlacarteContainer* AlacarteContainer.
   */
  AlacarteContainer *setAC();

  /**
   * @brief Delete ManagementContainer.
   *
   */
  void clearMC();

  /**
   * @brief Delete SituationContainer.
   *
   */
  void clearSC();

  /**
   * @brief Delete LocationContainer.
   *
   * @return voic
   */
  void clearLC();

  /**
   * @brief Delete AlacarteContainer.
   *
   */
  void clearAC();

  friend void swap(DENMWrapper &first, DENMWrapper &second) {
    using std::swap;
    swap(first._denm, second._denm);
  }
};

} // namespace denm

#define DENM_TYPE_NAME "DENM"

template <> struct TypeName<DENM> {
  static const char *get() { return DENM_TYPE_NAME; }
};

template <> struct TypeName<denm::DENMWrapper> {
  static const char *get() { return DENM_TYPE_NAME; }
};

} // namespace level