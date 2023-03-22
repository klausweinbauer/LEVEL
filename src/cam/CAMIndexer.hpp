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

#include <CAMWrapper.hpp>
#include <IDXIndex.hpp>
#include <IDXParameter.hpp>
#include <Indexer.hpp>
#include <QRYLatestMsg.hpp>
#include <unordered_map>

namespace level::cam {

class IDXCAMIndex : public IDXIndex<CAMWrapper> {};

class IDXCAMStationID : public IDXParameter<CAMWrapper, unsigned int> {
public:
  virtual unsigned int getValue(const CAMWrapper &entry) const {
    return entry->header.stationID;
  };
};

class IDXCAMLatest : public Indexer<CAMWrapper, QRYLatestMsg> {
private:
  std::unordered_map<unsigned int, unsigned int> _stationIDToIndex;
  std::unordered_map<unsigned int, unsigned int> _indexToStationID;

public:
  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<QRYLatestMsg> query) const override {
    if (_stationIDToIndex.count(query->stationID)) {
      return std::vector<unsigned int>{_stationIDToIndex.at(query->stationID)};
    } else {
      return std::vector<unsigned int>();
    }
  }

  /**
   * @brief Overrides the message stored as latest message.
   *
   * @throw Exception If index already exists.
   *
   * @param entry This will be the new latest message for its station id.
   * @param index Where the new latest message is stored.
   */
  virtual void addData(const CAMWrapper &entry, unsigned int index) override {
    if (_indexToStationID.count(index)) {
      std::stringstream ss;
      ss << "Entry with index '" << index << "' already exists." << std::endl;
      throw Exception(ERR, ss.str());
    }

    auto stationID = entry->header.stationID;

    if (_stationIDToIndex.count(stationID)) {
      auto oldIndex = _stationIDToIndex.at(stationID);
      _indexToStationID.erase(oldIndex);
      _stationIDToIndex.erase(stationID);
    }

    _indexToStationID[index] = stationID;
    _stationIDToIndex[stationID] = index;
  }

  /**
   * @brief If a message gets updated, it will be the new latest message for its
   * station id. If the station id changes, the old station id won't have a
   * latest message anymore. The message will be the latest message for the new
   * station id.
   *
   * @throw Exception If index does not exist.
   *
   * @param entry New latest message for its station id.
   * @param index Where the new latest message is stored.
   */
  virtual void updateData(const CAMWrapper &entry,
                          unsigned int index) override {
    if (_indexToStationID.count(index)) {
      auto stationID = entry->header.stationID;
      // StationID changed, remove latest message for old station id
      if (_indexToStationID[index] != stationID) {
        auto oldStationID = _indexToStationID[index];
        _stationIDToIndex.erase(oldStationID);
      }

      _stationIDToIndex[stationID] = index;
    } else {
      addData(entry, index);
    }
  }

  /**
   * @brief Remove a message. If the message was marked as latest message for a
   * specific station, the corresponding station won't have a latest message
   * anymore.
   *
   * @throw Exception If index does not exist.
   *
   * @param entry Message to remove.
   * @param index Index of the message.
   */
  virtual void removeData(const CAMWrapper &entry,
                          unsigned int index) override {
    if (_indexToStationID.count(index)) {
      auto stationID = _indexToStationID[index];
      _indexToStationID.erase(index);
      _stationIDToIndex.erase(stationID);
    }
  }

  /**
   * @brief Checks if the message is still marked as the latest message.
   *
   * @param entry Message to check.
   * @param index Index where the database thinks the latest message is stored.
   * @return true Entry is still the latest message for its station id.
   * @return false The entry is no longer marked as the latest message.
   */
  virtual bool isValid(const CAMWrapper &entry,
                       unsigned int index) const override {
    if (_indexToStationID.count(index)) {
      return _indexToStationID.at(index) == entry->header.stationID;
    } else {
      return false;
    }
  }
};

} // namespace level::cam