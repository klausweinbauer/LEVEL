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

namespace level {

class IPOTI {
public:
  virtual ~IPOTI() {}

  /**
   * @brief Current timestamp in milliseconds since 2004-01-01 00:00:00:000 UTC.
   *
   * @return unsigned long long int Timestamp in ms.
   */
  virtual unsigned long long int now() const = 0;
};

} // namespace level