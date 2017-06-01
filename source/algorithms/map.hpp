#pragma once

#include <algorithm>
#include <list>
#include <vector>

namespace braid::algorithms {

template<typename To, typename Iterator>
inline const To mapContainer(Iterator begin, Iterator end) {
  To transformed;

  std::copy(begin, end, std::back_inserter(transformed));

  return transformed;
}

template<typename To, typename From>
inline const std::list<To> mapContainer(const std::list<From>& l) {
  return mapContainer<std::list<To>>(l.cbegin(), l.cend());
}

template<typename To, typename From>
inline const std::vector<To> mapContainer(const std::vector<From>& v) {
  return mapContainer<std::vector<To>>(v.cbegin(), v.cend());
}

} // namespace braid::algorithms