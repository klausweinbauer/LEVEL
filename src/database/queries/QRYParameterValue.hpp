#pragma once

#include <IQRYParameterValue.hpp>

namespace level {

/**
 * @brief Query to get database elements by a specific parameter value.
 *
 * @tparam T Type of parameter. Must implement a default constructor.
 */
template <typename T> class QRYParameterValue : public IQRYParameterValue {
private:
  T _value;

public:
  /**
   * @brief Construct a new parameter value query.
   *
   */
  QRYParameterValue() : _value(T()) {}

  /**
   * @brief Construct a new parameter value query.
   *
   * @param value The parameter value the query should look for.
   */
  QRYParameterValue(T value) : _value(value) {}

  /**
   * @brief Parameter value used in this query.
   *
   * @return T Type of parameter.
   */
  T value() const { return _value; }
};

} // namespace level