#pragma once

namespace level {

class IQuery {
private:
  const int _queryType;

public:
  IQuery(int queryType) : _queryType(queryType){};
  virtual ~IQuery(){};

  virtual int getQueryType() { return _queryType; };
};

} // namespace level