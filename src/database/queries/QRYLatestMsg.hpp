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