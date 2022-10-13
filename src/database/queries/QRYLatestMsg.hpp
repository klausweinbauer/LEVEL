#pragma once

#include <IQuery.hpp>

namespace level {

class QRYLatestMsg : public IQuery {
public:
  unsigned int stationID;

  QRYLatestMsg(unsigned int stationID) : stationID(stationID) {}

  /**
   * @brief Create a query to get the latest message from a specific station.
   *
   * @param stationID Value of the parameter.
   * @return std::shared_ptr<QRYLatestMsg> Returns a query.
   */
  static std::shared_ptr<QRYLatestMsg> byId(unsigned int stationID) {
    return std::make_shared<QRYLatestMsg>(stationID);
  }
};

} // namespace level