#pragma once

#include <Indexer.hpp>
#include <QRYParameterValue.hpp>
#include <unordered_map>

namespace level {

template <typename TData, typename TParameter>
class ParameterIndexer : public Indexer<TData, IQRYParameterValue> {

private:
  // std::unordered_map<TParameter, std::vector<unsigned int>> _map;

public:
  virtual TParameter getValue(const TData &entry) = 0;

  virtual bool supportsQuery(std::shared_ptr<IQuery> query) override {
    bool supportsQuery =
        Indexer<TData, IQRYParameterValue>::supportsQuery(query);
    bool supportsParameter =
        dynamic_cast<QRYParameterValue<TParameter> *>(query.get());
    return supportsQuery && supportsParameter;
  };

  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<IQRYParameterValue> query) override {
    if (!supportsQuery(query)) {
      throw Exception(ERR_INVALID_ARG,
                      "This query is not supported by parameter indexer.");
    }

    std::shared_ptr<QRYParameterValue<TParameter>> parameterQuery =
        std::static_pointer_cast<QRYParameterValue<TParameter>>(query);
    return getByParameter(parameterQuery);
  }

  virtual std::vector<unsigned int>
  getByParameter(std::shared_ptr<QRYParameterValue<TParameter>> query) {
    // return _map[query->value()];
  }

  virtual void addData(const TData &entry, unsigned int index) {
    /*TParameter value = getValue(entry);
    std::pair<TParameter, std::vector<unsigned int>> mapEntry(value, index);
    _map.insert(mapEntry);*/
  }

  virtual void removeData(const TData &entry, unsigned int index) {}
};

} // namespace level