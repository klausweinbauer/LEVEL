#pragma once

#include <Indexer.hpp>
#include <QRYParameter.hpp>
#include <set>
#include <unordered_map>

namespace level {

/**
 * @brief Indexer to lookup database entries by specific parameter values in
 * O(1).
 *
 * @tparam TData Type of database entry.
 * @tparam TParameter Type of parameter used by queries to retrieve database
 * entries. This type must implement a copy constructor, assignment operator
 * (operator=), equality operator (operator==) and a hash function (struct
 * std::hash<TParameter>). Have a look at
 * https://en.cppreference.com/w/cpp/utility/hash for a good starting point.
 */
template <typename TData, typename TParameter>
class ParameterIndexer : public Indexer<TData, IQRYParameter> {

private:
  std::unordered_map<TParameter, std::set<unsigned int>> _map;

public:
  ParameterIndexer() {}

  /**
   * @brief Should return the parameter derived from the data element.
   *
   * @param entry
   * @return TParameter Parameter from data object.
   */
  virtual TParameter getValue(const TData &entry) = 0;

  virtual bool supportsQuery(std::shared_ptr<IQuery> query) override {
    bool supportsQuery = Indexer<TData, IQRYParameter>::supportsQuery(query);
    bool supportsParameter =
        dynamic_cast<QRYParameter<TParameter> *>(query.get());
    return supportsQuery && supportsParameter;
  };

  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<IQRYParameter> query) override {
    if (!supportsQuery(query)) {
      throw Exception(ERR_INVALID_ARG,
                      "This query is not supported by parameter indexer.");
    }

    std::shared_ptr<QRYParameter<TParameter>> parameterQuery =
        std::static_pointer_cast<QRYParameter<TParameter>>(query);
    return getByParameter(parameterQuery);
  }

  virtual std::vector<unsigned int>
  getByParameter(std::shared_ptr<QRYParameter<TParameter>> query) {
    TParameter key = query->value();
    if (_map.count(key)) {
      std::set<unsigned int> set = _map[key];
      std::vector<unsigned int> indices(set.size());
      std::copy(set.begin(), set.end(), indices.begin());
      return indices;
    } else {
      return std::vector<unsigned int>();
    }
  }

  virtual void addData(const TData &entry, unsigned int index) {
    TParameter key = getValue(entry);
    auto result = _map[key].insert(index);
    if (!result.second) {
      throw Exception(
          ERR, "Entry with this index already exists in parameter indexer.");
    }
  }

  virtual void removeData(const TData &entry, unsigned int index) {
    TParameter key = getValue(entry);
    if (_map.count(key)) {
      _map[key].erase(_map[key].find(index));
    }
  }
};

} // namespace level