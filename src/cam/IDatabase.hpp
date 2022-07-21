/**
 * @file IDatabase.hpp
 * @author Klaus Weinbauer
 * @brief Interface for CAM storage
 * @version 0.1
 * @date 2022-06-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <CAMData.hpp>
#include <DBView.hpp>
#include <memory>

namespace level {
namespace cam {

/**
 * @brief Interface for CAM storage
 *
 * @details This class provides the interface to the data access layer. Methods
 * that access an object (like get() or create()) return a \ref DBView
 * "DBView<CAMData>" object, which guarantees mutual exclusion for the requested
 * object for the lifetime of the view object. It is important to note that each
 * thread may only have one open view at any time because multiple views could
 * lead to deadlocks (the database raises a DBException if you try to).
 */
class IDatabase {

public:
  virtual ~IDatabase(){};

  /**
   * @brief Checks if an entry exists for the given id.
   *
   * @details Keep in mind that the return value is not thread-save and that the
   * database state might have changed when reading the value.
   *
   * @param stationID StationId of CAM.
   * @return true If CAM exists.
   * @return false If CAM does not exist.
   */
  virtual bool exists(long unsigned int stationID) = 0;

  /**
   * @brief Get a stored CAM by its stationID.
   *
   * @param stationID StationId of CAM.
   * @return DBView<CAMData> Returns requested CAM object. DBView wrapper
   * ensures mutual exclusion for the CAM object. Use the -> operator to access
   * the actual data.
   */
  virtual DBView<CAMData> get(long unsigned int stationID) = 0;

  /**
   * @brief Create a new CAM with stationID.
   *
   * @param stationID StationID of new CAM.
   * @return DBView<CAMData> Returns newly created CAM object. DBView wrapper
   * ensures mutual exclusion for the CAM object. Use the -> operator to access
   * the actual data.
   */
  virtual DBView<CAMData> create(long unsigned int stationID) = 0;

  /**
   * @brief Delete the CAM with the provided stationID.
   *
   * @param stationID StationID of CAM to delete.
   */
  virtual void remove(long unsigned int stationID) = 0;

  /**
   * @brief Returns the amount of messages stored in the database.
   *
   * @details Keep in mind that the return value is not thread-save and that the
   * database state might have changed when reading the value.
   *
   * @return size_t Number of CAMs.
   */
  virtual size_t entryCount() = 0;
};

}; // namespace cam
} // namespace level