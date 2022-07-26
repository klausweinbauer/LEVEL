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
  std::unordered_map<unsigned int, TParameter> _reverseLookup;

public:
  ParameterIndexer() {}

  /**
   * @brief Should return the parameter derived from the data element.
   *
   * @param entry
   * @return TParameter Parameter from data object.
   */
  virtual TParameter getValue(const TData &entry) const = 0;

  virtual bool supportsQuery(std::shared_ptr<IQuery> query) const override {
    bool supportsQuery = Indexer<TData, IQRYParameter>::supportsQuery(query);
    bool supportsParameter =
        dynamic_cast<QRYParameter<TParameter> *>(query.get());
    return supportsQuery && supportsParameter;
  };

  virtual std::vector<unsigned int>
  getIndexList(std::shared_ptr<IQRYParameter> query) const override {
    if (!supportsQuery(query)) {
      throw Exception(ERR_INVALID_ARG,
                      "This query is not supported by parameter indexer.");
    }

    std::shared_ptr<QRYParameter<TParameter>> parameterQuery =
        std::static_pointer_cast<QRYParameter<TParameter>>(query);
    return getByParameter(parameterQuery);
  }

  virtual std::vector<unsigned int>
  getByParameter(std::shared_ptr<QRYParameter<TParameter>> query) const {
    TParameter key = query->value();
    if (_map.count(key)) {
      std::set<unsigned int> set = _map.at(key);
      std::vector<unsigned int> indices(set.size());
      std::copy(set.begin(), set.end(), indices.begin());
      return indices;
    } else {
      return std::vector<unsigned int>();
    }
  }

  virtual void addData(const TData &entry, unsigned int index) {
    if (_reverseLookup.count(index)) {
      std::stringstream ss;
      ss << "Entry with index '" << index << "' already exists." << std::endl;
      throw Exception(ERR, ss.str());
    }

    TParameter key = getValue(entry);
    _map[key].insert(index);
    _reverseLookup[index] = key;
  }

  virtual void updateData(const TData &entry, unsigned int index) {
    if (!_reverseLookup.count(index)) {
      std::stringstream ss;
      ss << "There is no entry to update with index '" << index << "'."
         << std::endl;
      throw Exception(ERR, ss.str());
    }

    TParameter oldKey = _reverseLookup[index];
    TParameter newKey = getValue(entry);
    _map[oldKey].erase(_map[oldKey].find(index));
    _map[newKey].insert(index);
    _reverseLookup[index] = newKey;
  }

  virtual void removeData(const TData &entry, unsigned int index) {
    TParameter key = getValue(entry);
    if (_map.count(key)) {
      _map[key].erase(_map[key].find(index));
      _reverseLookup.erase(index);
    }
  }

  virtual bool isValid(const TData &entry, unsigned int index) const {
    TParameter key = getValue(entry);
    if (_reverseLookup.count(index)) {
      return _reverseLookup.at(index) == key;
    }
    return false;
  }
};

} // namespace level