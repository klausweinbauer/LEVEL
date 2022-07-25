#pragma once

#include <DBException.hpp>
#include <IIndexer.hpp>

namespace level {

template <typename TData, typename TQuery>
class Indexer : public IIndexer<TData> {

public:
  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<TQuery> query) = 0;

  virtual bool supportsQuery(std::shared_ptr<IQuery> query) override {
    return dynamic_cast<TQuery *>(query.get());
  };

  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<IQuery> query) override {
    auto castedQuery = dynamic_cast<TQuery *>(query.get());
    if (!castedQuery) {
      throw DBException(ERR_INVALID_ARG,
                        "The query is not derived from the defined query type "
                        "used in this indexer.");
    }
    std::shared_ptr<TQuery> derivedQuery =
        std::static_pointer_cast<TQuery>(query);
    return getIndexList(derivedQuery);
  }

  virtual void addData(const TData &entry, unsigned int index) {}

  virtual void removeData(const TData &entry, unsigned int index) {}
};

} // namespace level
