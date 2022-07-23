#pragma once

#include <IQuery.hpp>

namespace level {

class Query : public IQuery {
private:
  const int _queryType;

public:
  Query(int queryType) : _queryType(queryType){};
  virtual ~Query(){};

  virtual int getQueryType() const { return _queryType; };
};

} // namespace level