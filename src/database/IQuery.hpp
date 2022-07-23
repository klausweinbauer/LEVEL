#pragma once

namespace level {

class IQuery {
public:
  virtual ~IQuery(){};

  virtual int getQueryType() const = 0;
};

} // namespace level