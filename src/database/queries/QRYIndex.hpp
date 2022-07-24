#pragma once

#include <IQuery.hpp>

namespace level {

class QRYIndex : public IQuery {

private:
  unsigned int _firstIndex;
  unsigned int _lastIndex;

public:
  QRYIndex() : _firstIndex(0), _lastIndex(0) {}

  /**
   * @brief Construct a new index query object.
   *
   * @param index The index this query should retrieve.
   */
  QRYIndex(unsigned int index) : _firstIndex(0), _lastIndex(0) {
    setIndex(index);
  }

  /**
   * @brief Construct a new range index query object.
   *
   * @throw Exception if last index is smaller than first index.
   *
   * @param firstIndex Start index for this query (inclusive).
   * @param lastIndex Last index for this query (exclusive).
   */
  QRYIndex(unsigned int firstIndex, unsigned int lastIndex)
      : _firstIndex(0), _lastIndex(0) {
    setRange(firstIndex, lastIndex);
  }

  /**
   * @brief Construct a new range index query object by start index and length.
   * Negative length sets indices starting from startIndex (inclusive)
   * backwards.
   *
   * @param startIndex Start index for this query (inclusive).
   * @param length Number of indices for this query. Use negative values for
   * previous indices to startIndex.
   */
  QRYIndex(unsigned int startIndex, int length)
      : _firstIndex(0), _lastIndex(0) {
    setRange(startIndex, length);
  }

  /**
   * @brief Returns the first index used in this query (inclusive).
   *
   * @return unsigned int First index (inclusive).
   */
  unsigned int firstIndex() const { return _firstIndex; }

  /**
   * @brief Returns the last index used in this query (exclusive).
   *
   * @return unsigned int Last index (exclusive).
   */
  unsigned int lastIndex() const { return _lastIndex; }

  /**
   * @brief Returns the number of indices used for this query.
   *
   * @return unsigned int Number of indices.
   */
  unsigned int length() const { return _lastIndex - _firstIndex; }

  /**
   * @brief Set the first index for this query (inclusive).
   *
   * @throw Exception if index is greater than last index.
   *
   * @param index First index (inclusive).
   */
  void setFirstIndex(unsigned int index) {
    if (index > _lastIndex) {
      throw Exception(ERR, "First index is greater than last index.");
    }
    _firstIndex = index;
  }

  /**
   * @brief Set the last index for this query (exclusive).
   *
   * @throw Exception if index is smaller than first index.
   *
   * @param index Last index (exclusive).
   */
  void setLastIndex(unsigned int index) {
    if (index < _firstIndex) {
      throw Exception(ERR, "Last index is smaller than first index.");
    }
    _lastIndex = index;
  }

  /**
   * @brief Set a single index for this query.
   *
   * @param index Single index to query.
   */
  void setIndex(unsigned int index) {
    if (index == UINT32_MAX) {
      throw Exception(ERR, "Index overflow.");
    }

    _firstIndex = index;
    _lastIndex = index + 1;
  }

  /**
   * @brief Set a range of indexes. Negative length sets indices starting from
   * startIndex (inclusive) backwards.
   *
   * @param startIndex Start index for this query (inclusive).
   * @param length Number of indices for this query. Use negative values for
   * previous indices to startIndex.
   */
  void setRange(unsigned int startIndex, int length) {
    long long int iLow = (length < 0) ? startIndex + 1 : startIndex;
    long long int iHigh = iLow + length;

    // swap iLow and iHigh
    if (iLow > iHigh) {
      iLow = iLow + iHigh;
      iHigh = iLow - iHigh;
      iLow = iLow - iHigh;
    }

    if (iLow < 0) {
      throw Exception(ERR, "Index underflow.");
    }
    if (iHigh > UINT32_MAX) {
      throw Exception(ERR, "Index overflow.");
    }

    _firstIndex = iLow;
    _lastIndex = iHigh;
  }

  /**
   * @brief Set a range of indexes by first and last index.
   *
   * @throw Exception if last index is smaller than first index.
   *
   * @param firstIndex First index (inclusive).
   * @param lastIndex Last index (exclusive).
   */
  void setRange(unsigned int firstIndex, unsigned int lastIndex) {
    if (lastIndex < firstIndex) {
      throw Exception(ERR_INVALID_ARG,
                      "Last index is smaller than first index.");
    }
    _firstIndex = firstIndex;
    _lastIndex = lastIndex;
  }

  /**
   * @brief Create a query with a range of indexes. Negative length sets indices
   * starting from startIndex (inclusive) backwards.
   *
   * @param startIndex Start index for this query (inclusive).
   * @param length Number of indices for this query. Use negative values for
   * previous indices to startIndex.
   */
  static QRYIndex byRange(unsigned int startIndex, int length) {
    return QRYIndex(startIndex, length);
  }

  /**
   * @brief Create a query for a range of indexes by first and last index.
   *
   * @param firstIndex First index (inclusive).
   * @param lastIndex Last index (exclusive).
   */
  static QRYIndex byRange(unsigned int firstIndex, unsigned int lastIndex) {
    return QRYIndex(firstIndex, lastIndex);
  }

  /**
   * @brief Create a query for a single index.
   *
   * @param index Single index to query.
   */
  static QRYIndex byIndex(unsigned int index) { return QRYIndex(index); }
};

} // namespace level