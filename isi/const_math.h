#pragma once

#include <cmath>
#include <type_traits>
#include <algorithm>

template<class t>
concept Number = std::is_arithmetic<t>::value;
template<class t>
concept Floating = std::is_floating_point<t>::value;

template<Number t>
t c_pow(t a, t b) {
	return static_cast<t>(std::pow(a, b));
}

template<Number t>
t c_sqrt(t a) {
	return static_cast<t>(std::sqrt(a));
}