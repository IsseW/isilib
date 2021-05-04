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

template<>
float c_pow<float>(float a, float b) {
	return std::powf(a, b);
}

template<Number t>
t c_sqrt(t a) {
	return static_cast<t>(std::sqrt(a));
}

template<>
float c_sqrt<float>(float a) {
	return std::sqrtf(a);
}
template<class t, class _t>
constexpr t cmin(t a, _t b) {
	return a < b ? a : b;
}
template<class t, class... Args>
constexpr t cmin(t a, Args... args) requires (sizeof...(Args) >= 2) {
	return cmin(a, cmin(args...));
}

template<class t>
constexpr t cmax(t a, t b) {
	return a > b ? a : b;
}
template<class t, class... Args>
constexpr t cmax(t a, Args... args) requires (sizeof...(Args) >= 2) {
	return cmax(a, cmax(args...));
}

template<class A, class B, class C>
constexpr A cclamp(const A& value, const B& min, const C& max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}