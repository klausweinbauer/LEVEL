#pragma once

#include <DENMWrapper.hpp>
#include <IDXIndex.hpp>
#include <IDXParameter.hpp>
#include <level.h>

namespace level::denm {

class IDXDENMIndex : public IDXIndex<DENMWrapper> {};

class IDXDENMStationID : public IDXParameter<DENMWrapper, unsigned int> {
public:
  virtual unsigned int getValue(const DENMWrapper &entry) const {
    return entry->header.stationID;
  };
};

} // namespace level::denm