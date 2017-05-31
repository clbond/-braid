#pragma once

#include <algorithm>
#include <list>
#include <vector>

#include "algorithms/map.hpp"

namespace braid::algorithms {

template<typename To, typename Iterator>
inline const To map(Iterator begin, Iterator end) {
  To transformed;

  std::copy(begin, end, std::back_inserter(transformed));

  return transformed;
}

template<typename To, typename From>
inline const std::list<To> map(const std::list<From>& l) {
  return map<std::list<To>>(l.cbegin(), l.cend());
}

template<typename To, typename From>
inline const std::vector<To> map(const std::vector<From>& v) {
  return map<std::vector<To>>(v.cbegin(), v.cend());
}

} // namespace braid::algorithms