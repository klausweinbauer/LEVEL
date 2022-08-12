#pragma once

#include <CAMWrapper.hpp>
#include <ParameterIndexer.hpp>

namespace level::cam {

class IDXCAMStationID : public ParameterIndexer<CAMWrapper, unsigned int> {
public:
  virtual unsigned int getValue(const CAMWrapper &entry) const {
    return entry->header.stationID;
  };
};

} // namespace level::cam