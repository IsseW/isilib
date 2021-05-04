#pragma once
#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <array>

#include "const_math.h"

#include "macros.h"

#pragma region macros

#define VECTOR_ELEMENT(name, index)											\
  constexpr t &name() requires (l > index) { return get<index>(); } \
  constexpr const t &_##name() const requires (l > index) { return get<index>(); }

#define FUNC_CALL(x) _##x()

#define ELE_V(...)                                                      \
  constexpr vec<NARGS(__VA_ARGS__), t> NO_SPACES(__VA_ARGS__)() const { \
    return vec<NARGS(__VA_ARGS__), t>(APPLY(FUNC_CALL, __VA_ARGS__));   \
  }

#define ELE_V2_(e) ELE_V(e, x) ELE_V(e, y) ELE_V(e, z) ELE_V(e, w) ELE_V(e, o)
#define ELE_V2 ELE_V2_(x) ELE_V2_(y) ELE_V2_(z) ELE_V2_(w) ELE_V2_(o)

#define ELE_V3__(a, b) \
  ELE_V(a, b, x) ELE_V(a, b, y) ELE_V(a, b, z) ELE_V(a, b, w) ELE_V(a, b, o)
#define ELE_V3_(e) \
  ELE_V3__(e, x) ELE_V3__(e, y) ELE_V3__(e, z) ELE_V3__(e, w) ELE_V3__(e, o)
#define ELE_V3 ELE_V3_(x) ELE_V3_(y) ELE_V3_(z) ELE_V3_(w) ELE_V3_(o)

#define ELE_V4___(a, b, c) \
  ELE_V(a, b, c, x)        \
  ELE_V(a, b, c, y) ELE_V(a, b, c, z) ELE_V(a, b, c, w) ELE_V(a, b, c, o)
#define ELE_V4__(a, b) \
  ELE_V4___(a, b, x)   \
  ELE_V4___(a, b, y) ELE_V4___(a, b, z) ELE_V4___(a, b, w) ELE_V4___(a, b, o)
#define ELE_V4_(e) \
  ELE_V4__(e, x) ELE_V4__(e, y) ELE_V4__(e, z) ELE_V4__(e, w) ELE_V4__(e, o)
#define ELE_V4 ELE_V4_(x) ELE_V4_(y) ELE_V4_(z) ELE_V4_(w) ELE_V4_(o)

#define STANDARD_SUB_VECTORS ELE_V2 ELE_V3 ELE_V4

#define VECTOR_C_OPERATOR(op)                                     \
  template <typename t_>                                          \
  constexpr vec<l, bool> operator op(const vec<l, t_> &other) const { \
    vec<l, bool> result;                                              \
    for (size_t i = 0; i < l; i++) {								  \
      result[i] = e[i] op other[i];									  \
    }																  \
    return result;													  \
  }																	  \
  template <typename t_>											  \
  constexpr vec<l, bool> operator op(const t_ &other) const {		  \
    vec<l, bool> result;											  \
    for (size_t i = 0; i < l; i++) {								  \
      result[i] = e[i] op other;									  \
    }																  \
    return result;													  \
  }

#define VECTOR_COPY_OPERATOR(op)											   \
  template <size_t l_, typename t_>											   \
  constexpr auto operator op(const vec<l_, t_> &other) const {				   \
    vec<l, t> result;														   \
    for (size_t i = 0; i < l; i++) {										   \
      if (i < l_) {															   \
        result[i] = e[i] op other[i];									       \
      } else {																   \
        result[i] = e[i];													   \
      }																		   \
    }																		   \
    return result;															   \
  }																			   \
  template <typename t_>													   \
  constexpr auto operator op(const t_ &other) const {						   \
    vec<l, t> result;														   \
    for (size_t i = 0; i < l; i++) {										   \
      result[i] = e[i] op other;											   \
    }																		   \
    return result;															   \
  }

#define VECTOR_ASSIGN_OPERATOR(op)									   \
  template <size_t l_, typename t_>									   \
  constexpr vec<l, t> &operator CAT(op, =)(const vec<l_, t_> &other) { \
    constexpr int max = std::min(l, l_);                               \
    for (size_t i = 0; i < max; i++) {                                 \
      e[i] CAT(op, =) other[i];                                        \
    }                                                                  \
    return *this;                                                      \
  }                                                                    \
  template <typename t_>                                               \
  constexpr vec<l, t> &operator CAT(op, =)(const t_ &other) {          \
    for (size_t i = 0; i < l; i++) {                                   \
      e[i] CAT(op, =) other;                                           \
    }                                                                  \
    return *this;                                                      \
  }

#define VECTOR_OPERATOR(op) VECTOR_COPY_OPERATOR(op) VECTOR_ASSIGN_OPERATOR(op)

#define VECTOR_S_OPERATOR(op)          \
  constexpr auto operator op() const { \
    vec<l, t> result;                    \
    for (size_t i = 0; i < l; i++) {   \
      result[i] = op e[i];             \
    }                                  \
    return result;                     \
  }

#define VECTOR(c) template<typename t> using v##c = vec<c, t>
#pragma endregion

#ifndef DEFAULT_FLOATING
#define DEFAULT_FLOATING float
#endif // !DEFAULT_FLOATING

struct _vector {

};

template <size_t l, typename t = DEFAULT_FLOATING>
struct vec : _vector {
private:
	std::array<t, l> e{0};

    /// TODO: move a lot of this of this into _vector
    constexpr size_t size() const { return l; }

	/// <summary>
	/// Create a vector with all of one element.
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	static constexpr vec<l, t> allof(t val) {
		vec<l, t> r;
        std::fill(r.begin(), r.end(), val);
		return r;
	}

public:
	/// <summary>
	/// A vector with only 0s.
	/// </summary>
	/// <returns></returns>
	static constexpr vec<l, t> zero() requires std::is_arithmetic<t>::value {
		return allof(t{0});
	}
	/// <summary>
	/// A vector filled with ones.
	/// </summary>
	/// <returns></returns>
	static constexpr vec<l, t> one() requires std::is_arithmetic<t>::value {
		return allof(t{1});
	}

	/// <summary>
	/// One minus for each element.
	/// </summary>
	/// <returns></returns>
	constexpr vec<l, t> frac() const requires std::is_arithmetic<t>::value {
		vec<l, t> result;
		for (size_t i = 0; i < l; i++) {
			result[i] = t{ 1 } - e[i];
		}
		return result;
	}
	/// <summary>
	/// Clamp vector values.
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	constexpr vec<l, t> clamp(const t& min, const t& max) const {
		vec<l, t> result;
		for (size_t i = 0; i < l; i++) {
			result[i] = std::clamp(e[i], min, max);
		}
		return result;
	}
	/// <summary>
	/// Round vector values.
	/// </summary>
	/// <returns></returns>
	constexpr vec<l, t> round() const {
		vec<l, t> result;
		for (size_t i = 0; i < l; i++) {
			result[i] = std::round(e[i]);
		}
		return result;
	}
	/// <summary>
	/// Floor vector values.
	/// </summary>
	/// <returns></returns>
	constexpr vec<l, t> floor() const {
		vec<l, t> result;
		for (size_t i = 0; i < l; i++) {
			result[i] = std::floor(e[i]);
		}
		return result;
	}
	/// <summary>
	/// Ceiling vector values.
	/// </summary>
	/// <returns></returns>
	constexpr vec<l, t> ceil() const {
		vec<l, t> result;
		for(size_t i = 0; i < l; i++) {
			result[i] = std::ceil(e[i]);
		}
		return result;
	}

	/// <summary>
	/// Get a vector with the sign of each element.
	/// </summary>
	/// <returns></returns>
	constexpr vec<l, t> sign() const requires std::is_signed_v<t> {
		vec<l, t> result;
		for(size_t i = 0; i < l; i++) {
			result[i] = e[i] & (0b1 << sizeof(t)*8-1) +1 ;
		}
		return result;
	}
    /// <summary>
    /// Default vector constructor, initializes every element have a value of zero,
    constexpr vec() {}
    /// </summary>
	/// <summary>
	/// Constructor for constructing each vector element with a certain value.
	/// </summary>
	constexpr vec(t v) {
        std::fill(e.begin(), e.end(), v);
    }
    /// <summary>
    /// initializes every element of the vactor to any given value corresponding the the vector type
    /// </summary>
	/// <summary>
	/// Copy constructor.
	/// </summary>
	/// <param name="other"></param>
	constexpr vec(const vec<l, t>& other) {
        std::copy(other.e.begin(), other.e.end(), e.begin());
	}
	/// <summary>
	/// Create vector from a subvector and a number of elements.
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="other"></param>
	/// <param name="...args"></param>
	template <size_t a, typename... Args>
	constexpr vec(const vec<a, t>& other, const Args&...args) requires (a + sizeof...(Args) == l) {
		size_t i = 0;
		for (; i < a; i++) {
			e[i] = other[i];
		}
		((e[i++] = static_cast<t>(args)), ...);
	}
	/// <summary>
	/// Create vector from two subvectors.
	/// </summary>
	/// <param name="_a"></param>
	/// <param name="_b"></param>
	template <size_t a, size_t b>
	constexpr vec(const vec<a, t>& _a, const vec<b, t>& _b) requires (a + b == l) {
		size_t i = 0;
		for (; i < a; i++) {
			e[i] = _a[i];
		}
		for (size_t j = 0; j < a; j++) {
			e[i + j] = _b[j];
		}
	}
	/// <summary>
	/// Create a vector from two subvectors and some elements.
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="_a"></param>
	/// <param name="_b"></param>
	/// <param name="...args"></param>
	template <size_t a, size_t b, typename... Args>
	constexpr vec(const vec<a, t>& _a, const vec<b, t>& _b, const Args&...args) requires (a + b + sizeof...(Args) == l) {
		size_t i = 0;
		for (; i < a; i++) {
			e[i] = _a[i];
		}
		size_t j = 0;
		for (; j < a; j++) {
			e[i + j] = _b[j];
		}
		i += j;
		((e[i++] = static_cast<t>(args)), ...);
	}
	/// <summary>
	/// Create vector from elements.
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	template <typename... Args>
	constexpr vec(const Args&...args) requires (sizeof...(Args) == l) {
		size_t loop = 0;
		((e[loop++] = static_cast<t>(args)), ...);
	}

	/// <summary>
	/// Copy operator
	/// </summary>
	/// <typeparam name="t_"></typeparam>
	/// <param name="other"></param>
	/// <returns></returns>
	template <int l_, typename t_>
	constexpr vec<l, t>& operator=(vec<l_, t_> const& other) {
        std::copy(other.e.begin(), other.e.end(), e.begin());
		return *this;
	}

	// For operator # and some type _t define v<t> # v<_t>, v<t> #= v<_t>, v<t> # _t and v<t> #= _t
	VECTOR_OPERATOR(+);
	VECTOR_OPERATOR(-);
	VECTOR_OPERATOR(*);
	VECTOR_OPERATOR(/ );
	VECTOR_OPERATOR(%);
	VECTOR_OPERATOR(&);
	VECTOR_OPERATOR(| );
	VECTOR_OPERATOR(^);

	// For operator # and some type _t define v<t> # v<_t> and v<t> # _t
	VECTOR_COPY_OPERATOR(&&);
	VECTOR_COPY_OPERATOR(|| );

	// For operator # define #v<t>

	VECTOR_S_OPERATOR(-);
	VECTOR_S_OPERATOR(~);
	VECTOR_S_OPERATOR(!);

	// Define equality vector operators, all returning a bool vector of all individual elements compared. either with elements of another vector or with a single element
	VECTOR_C_OPERATOR(== );
	VECTOR_C_OPERATOR(!= );
	VECTOR_C_OPERATOR(< );
	VECTOR_C_OPERATOR(> );
	VECTOR_C_OPERATOR(<= );
	VECTOR_C_OPERATOR(>= );

	// increment operators, increments each value individually. 
	constexpr vec<l, t> operator++() {
		vec<l, t> c = *this;
		for(auto& elem: e) {
		    elem++;
		}
		return c;
	}
	constexpr vec<l, t> operator--() {
		vec<l, t> c = *this;
		for(auto& elem: e) {
		    elem--;
		}
		return c;
	}

	constexpr vec<l, t>& operator++(int) {
		for(auto& elem: e) {
			++elem;
		}
		return *this;
	}
	constexpr vec<l, t>& operator--(int) {
		for(auto& elem: e) {
			--elem;
		}
		return *this;
	}

	/// <summary>
	/// Lerp this vector towards the goal.
	/// </summary>
	/// <param name="goal">The vector that this vector should lerp towards.</param>
	/// <param name="time">How far to lerp.</param>
	/// <returns>A reference to this.</returns>
	constexpr vec<l, t>& lerp_towards(const vec<l, t>& goal, double percent) {
        auto iter = goal.e.begin();
		std::for_each(e.begin(), e.end(), [&](t&elem){
			elem = std::lerp(elem, *(iter++), percent);
		});
		return *this;
	}

	/// <summary>
	/// Get the reverse of this vector. 
	/// </summary>
	/// <returns>A reversed version of this vector.</returns>
	constexpr vec<l, t> get_reverse() const {
		vec<l, t> result = *this;
		std::reverse(result.e.begin(), result.e.end());
		return result;
	}

    constexpr void reverse() {
        *this = get_reverse();
    }

	/// <summary>
	/// The square length of this vector
	/// </summary>
	/// <returns></returns>
	constexpr t length_sqr() const {
		t r{0};
		for(const auto& elem: e) {
			r += elem * elem;
		}
		return r;
	}
	/// <summary>
	/// The length of this vector
	/// </summary>
	/// <typeparam name="Returns">What type this function should return.</typeparam>
	/// <returns>Length of vector</returns>
	template<class Returns = t>
	Returns length() const { return std::sqrt(length_sqr()); }

	/// <summary>
	/// Returns a normalized version of this vector.
	/// </summary>
	/// <returns></returns>
	constexpr vec<l, t> normalized() const {
		return operator/(length());
	}
	/// <summary>
	/// Normalizes this vector.
	/// </summary>
	/// <returns></returns>
	constexpr vec<l, t> normalize() const {
		operator/=(length());
	}
	/// <summary>
	/// Check if all elements evaluate to a given value. returns true if they do, otherwise false. 
    /// Has a default parameter of 0.
	/// </summary>
	/// <returns></returns>
	constexpr bool all(const t value = 0) const {
		return std::find(e.begin(), e.end(), 0) == e.end();
	}
	/// <summary>
	/// Check if any element evaluate to true in an if statement. 
	/// </summary>
	/// <returns></returns>
	constexpr bool any() const {
		return std::find_if(e.begin(), e.end(), [](t v){ return v != 0; }) != e.end();
	}
	/// <summary>
	/// Check if any element equals the given value.
	/// </summary>
	/// <typeparam name="t_"></typeparam>
	/// <param name="value"></param>
	/// <returns></returns>
	constexpr bool any(const t& value) const {
		return std::find(e.begin(), e.end(), value) != e.end();
	}

	/// <summary>
	/// Find the smallest element in the vector.
	/// </summary>
	/// <returns></returns>
	constexpr t min_e() const {
		return *std::min_element(e.begin(), e.end());
	}

	/// <summary>
	/// Find the largest element in the vector.
	/// </summary>
	/// <returns></returns>
	constexpr t max_e() const {
        return *std::max_element(e.begin(), e.end());
	}

	/// <summary>
	/// The sum of the vector elements
	/// </summary>
	/// <returns></returns>
	constexpr t sum() const {
		t r{ 0 };
		for (const auto& elem: e) {
			r += elem;
		}
		return r;
	}

	/// <summary>
	/// Average of the vector elements
	/// </summary>
	/// <returns></returns>
	constexpr t average() const {
		return sum() / l;
	}

	/// <summary>
	/// All elements in the vector multiplied by eachother.
	/// </summary>
	/// <returns></returns>
	constexpr t volume() const {
		t r{ 1 };
		for (const auto& elem: e) {
			r *= elem;
		}
		return r;
	}

	// Print vector.
	friend std::ostream& operator<<(std::ostream& os, const vec<l, t>& vec) {
		os << '<';
		for (size_t i = 0; i < l; i++) {
			if (i) os << ", ";
			os << vec[i];
		}
		os << '>';
		return os;
	}

	// Index vector
	t& operator[](const int index) { return e[index]; }
	constexpr t& operator[](const int index) const { return e[index]; }

	/// <summary>
	/// Get a raw pointer to the vector elements
	/// </summary>
	/// <returns></returns>
	auto begin() { return e.begin(); }
	/// <summary>
	/// The pointer after the vector array. Looks illegal.
	/// </summary>
	/// <returns></returns>
	auto end() { return e.end(); }
	/// <summary>
	/// Const accessor for element reference.
	/// </summary>
	/// <returns></returns>
	template <size_t i>
	constexpr const t& get() const requires (i < l) {
		return e[i];
	}

	/// <summary>
	/// Static accessor for element reference.
	/// </summary>
	/// <returns></returns>
	template <size_t i>
	t& get() requires (i < l) {
		return e[i];
	}

	/// <summary>
	/// Conversion from one vector to another vector.
	/// </summary>
	/// <typeparam name="_t"></typeparam>
	/// <param name=""></param>
	template<size_t _l, typename _t>
	constexpr operator vec<_l, _t>(void) const {
		vec<_l, _t> result;
		constexpr size_t max = l < _l ? l : _l;
		for(size_t i = 0; i < max; ++i) {
			result[i] = static_cast<_t>(e[i]);
		}
		return result;
	}

	/// <summary>
	/// Creates a vector of the same size with the abs of each of this vectors elements.
	/// </summary>
	/// <returns></returns>
	constexpr vec<l, t> abs() const {
		vec<l, t> r;
		for(size_t i = 0; i < l; ++i) {
			r[i] = std::abs(e[i]);
		}
		return r;
	}

	/// <summary>
	/// Creates a new vector where each elements is decided between the max of the parameter and this vectors element.
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	constexpr vec<l, t> vmax(const t& a) const {
		vec<l, t> r;
		for(size_t i = 0; i < l; ++i) {
			r[i] = std::max(e[i], a);
		}
		return r;
	}
	/// <summary>
	/// Creates a new vector where each element is the max of that element in this vector and the parameter vector.
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	constexpr vec<l, t> vmax(const vec<l, t>& a) const {
		vec<l, t> r;
		for(size_t i = 0; i < l; i++){
			r[i] = std::max(e[i], a[i]);
		}
		return r;
	}

	/// <summary>
	/// Creates a new vector where each elements is decided between the min of the parameter and this vectors element.
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	constexpr vec<l, t> vmin(const t& a) const {
		vec<l, t> r;
		for (size_t i = 0; i < l; i++){
			r[i] = std::min(e[i], a);
		}
		return r;
	}
	/// <summary>
	/// Creates a new vector where each element is the min of that element in this vector and the parameter vector.
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	constexpr vec<l, t> vmin(const vec<l, t>& a) const {
		vec<l, t> r;
		for (size_t i = 0; i < l; i++){
			r[i] = std::min(e[i], a[i]);
		}
		return r;
	}

	/// <summary>
	/// Returns a vector with a 90 degree angle to this. If It's the zero vector it return the zero vector.
	/// </summary>
	/// <returns>Ortonogal vector</returns>
	vec<2, t> ortonogal() const requires (l == 2) { return v2<t>(e[1], -e[0]); }
	/// <summary>
	/// Returns a vector with a 90 degree angle to this. If It's the zero vector it return the zero vector.
	/// It is the negated vector of the other ortonogal function.
	/// </summary>
	/// <returns>Ortonogal vector</returns>
	vec<2, t> _ortonogal() const requires (l == 2) { return v2<t>(-e[1], e[0]); }

	/// <summary>
	/// Cross product for a 3d vector.
	/// </summary>
	/// <param name="o"></param>
	/// <returns></returns>
	vec<3, t> cross(const vec<3, t>& o) const requires (l == 3) {
		return vec<3, t>(e[1] * o[2] - e[2] * o[1], e[2] * o[0] - e[0] * o[2],
			e[0] * o[1] - e[1] * o[0]);
	}


	/// <summary>
	/// Cross product for a 7d vector
	/// </summary>
	/// <param name="o"></param>
	/// <returns></returns>
	vec<7, t> cross(const vec<7, t>& o) const requires (l == 7) {
		return vec<7, t>(
			e[1] * o[3] - e[3] * o[1] + e[2] * o[6] - e[6] * o[2] + e[4] * o[5] - e[5] * o[4],
			e[2] * o[4] - e[4] * o[2] + e[3] * o[0] - e[0] * o[3] + e[5] * o[6] - e[6] * o[7],
			e[3] * o[5] - e[5] * o[3] + e[4] * o[1] - e[1] * o[4] + e[6] * o[0] - e[0] * o[6],
			e[4] * o[6] - e[6] * o[4] + e[5] * o[2] - e[2] * o[6] + e[0] * o[1] - e[1] * o[0],
			e[5] * o[0] - e[0] * o[5] + e[6] * o[3] - e[3] * o[6] + e[1] * o[2] - e[2] * o[1],
			e[6] * o[1] - e[1] * o[6] + e[0] * o[4] - e[4] * o[0] + e[2] * o[4] - e[4] * o[2],
			e[0] * o[2] - e[2] * o[0] + e[1] * o[5] - e[5] * o[1] + e[3] * o[4] - e[4] * o[3]);
	}

	// Create accessors for elements. accessed with for example x() for reference and _x() for const reference.
	VECTOR_ELEMENT(x, 0);
	VECTOR_ELEMENT(y, 1);
	VECTOR_ELEMENT(z, 2);
	VECTOR_ELEMENT(w, 3);

	t _o() const { return t(); }

	// Create all combinations of vector elements including o for zero. For example zyx() or xzy()
	// goes up to 4 dimensions
	STANDARD_SUB_VECTORS;
};

// A vector specializeation for size 0 to avoid some problems.
// I see no reason to create this.
template<typename t>
struct vec<0, t> {

};

// Create typedefs for v2, v3, v4 etc.
VECTOR(2);
VECTOR(3);
VECTOR(4);
VECTOR(5);
VECTOR(6);
VECTOR(7);



/// <summary>
/// Dot product between two vectors.
/// </summary>
/// <typeparam name="t"></typeparam>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
template<size_t l, typename t>
constexpr t dot(const vec<l, t>& a, const vec<l, t>& b) {
	t result = t{};
	for (size_t i = 0; i < l; i++){
		result += a[i] * b[i];
	}
	return result;
}

template<size_t l, typename t, typename t_>
constexpr vec<l, t> operator*(const t& a, const vec<l, t>& b) {
	return b * a;
}

template<Floating t = DEFAULT_FLOATING>
struct quaternion {
	t x{ 0 };
	t y{ 0 };
	t z{ 0 };
	t w{ 0 };
	/// <summary>
	/// Get the identity quaternion
	/// </summary>
	/// <returns></returns>
	static constexpr quaternion<t> identity() { return quaternion<t>(0, 0, 0, 1); }
	constexpr quaternion() { }
	constexpr quaternion(const quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
	constexpr quaternion(const t& x, const t& y, const t& z, const t& w) : x(x), y(y), z(z), w(w) { }
	constexpr quaternion(const vec<4, t>& other) : x(other._x()), y(other._y()), z(other._z()), w(other._w()) { }

	constexpr static quaternion<t> from_euler(const vec<3, t>& other) {
		return from_euler(other._x(), other._y(), other._z());
	}
	/// <summary>
	/// Create quaternion from euler
	/// </summary>
	/// <param name="yaw"></param>
	/// <param name="pitch"></param>
	/// <param name="roll"></param>
	/// <returns></returns>
	constexpr static quaternion<t> from_euler(const t& yaw, const t& pitch, const t& roll) {
		t cy = cos(yaw / t{ 2 });
		t sy = sin(yaw / t{ 2 });
		t cp = cos(pitch / t{ 2 });
		t sp = sin(pitch / t{ 2 });
		t cr = cos(roll / t{ 2 });
		t sr = sin(roll / t{ 2 });

		return quaternion<t>(sr * cp * cy + cr * sp * sy,
			cr * sp * cy - sr * cp * sy,
			cr * cp * sy + sr * sp * cy,
			cr * cp * cy - sr * sp * sy);
	}
	/// <summary>
	/// Create euler vector from this quaternion
	/// </summary>
	/// <returns></returns>
	constexpr vec<3, t> to_euler() {
		vec<3, t> result;
		t t0, t1;

		t0 = t{ 2.0 } *(w * x + y * z);
		t1 = t{ 1.0 } *(x * x + y * y);
		result.x() = std::atan2(t0, t1);

		t0 = t{ 2.0 } *(w * y - x * z);
		t0 = std::clamp(t0, t{ -1.0 }, t{ 1.0 });
		result.y() = std::asin(t0);

		t0 = t{ 2.0 } *(w * z + x * y);
		t1 = t{ 1.0 } - t{ 2.0 } *(y * y + z * z);
		result.z() = std::atan2(t0, t1);

		return result;
	}
	/// <summary>
	/// Multiply quaternions
	/// </summary>
	/// <param name="o"></param>
	/// <returns></returns>
	constexpr quaternion<t> operator*(const quaternion<t>& o) const {
		return quaternion(w * o.x + x * o.w + y * o.z - z * o.y, // X
			w * o.y - x * o.z + y * o.w + z * o.x, // Y 
			w * o.z + x * o.y - y * o.x + z * o.w, // Z
			w * o.w - x * o.x - y * o.y - z * o.z  // W
		);
	}

	constexpr quaternion<t>& operator*=(const quaternion<t>& o) {
		x = w * o.x + x * o.w + y * o.z - z * o.y;
		y = w * o.y - x * o.z + y * o.w + z * o.x;
		z = w * o.z + x * o.y - y * o.x + z * o.w;
		w = w * o.w - x * o.x - y * o.y - z * o.z;
		return *this;
	}
	/// <summary>
	/// "Scale" quaternion
	/// </summary>
	/// <param name="o"></param>
	/// <returns></returns>
	constexpr quaternion<t> operator*(const t& o) const {
		return quaternion(x * o, y * o, z * o, w * o);
	}

	constexpr quaternion<t>& operator*=(const t& o) const {
		x *= o;
		y *= o;
		z *= o;
		w *= o;
		return *this;
	}
	/// <summary>
	/// Rotate vector by this quaternion
	/// </summary>
	/// <param name="u"></param>
	/// <returns></returns>
	constexpr vec<3, t> operator*(const vec<3, t>& u) const {
		// vec<3, t> v(x, y, z);
		quaternion<t> v(u._x(), u._y(), u._z(), t{ 0 });
		quaternion<t> result = (*this * v) * conjugate();
		return { result.x, result.y, result.z }; //u * (t{ 2.0 } * dot(u, v)) + v * (w * w - dot(u, u)) + u.cross(vec) * (t{ 2.0 } * w);
	}
	/// <summary>
	/// Get a vector for right in this current rotation.
	/// </summary>
	/// <returns></returns>
	constexpr vec<3, t> right() const {
		return vec<3, t>(x * t{ 2.0 } + x * (w * w - 1), y * (w * w - 1) - z * w * t{ 2.0 }, z * (w * w - 1) + y * w * t{ 2.0 });
	}
	/// <summary>
	/// Get a vector for up in this current rotation.
	/// </summary>
	/// <returns></returns>
	constexpr vec<3, t> up() const {
		return vec<3, t>(x * (w * w - 1) + z * w * t{ 2.0 }, y * (w * w - 1) + y * t{ 2.0 }, z * (w * w - 1) - x * w * t{ 2.0 });
	}

	/// <summary>
	/// Get a vector for forward in this current rotation.
	/// </summary>
	/// <returns></returns>
	constexpr vec<3, t> forward() const {
		return vec<3, t>(x * (w * w - 1) - y * t{ 2.0 } *w, y * (w * w - 1) + x * t{ 2.0 } *w, z * (w * w - 1) + z * t{ 2.0 });
	}
	/// <summary>
	/// Get the conjugate to this quaternion.
	/// </summary>
	/// <returns></returns>
	constexpr quaternion<t> conjugate() const {
		return quaternion(-x, -y, -z, w);
	}
	/// <summary>
	/// Print this quaternion.
	/// </summary>
	/// <param name="os"></param>
	/// <param name="q"></param>
	/// <returns></returns>
	friend std::ostream& operator<<(std::ostream& os, const quaternion<t>& q) {
		os << '<' << q.x << ", " << q.y << ", " << q.z << ", " << q.w << '>';
		return os;
	}
};

using quat = quaternion<>;

/// <summary>
/// Reflect a vector on a perfect mirror.
/// </summary>
/// <typeparam name="t"></typeparam>
/// <param name="d">The incoming direction</param>
/// <param name="n">Normal of surface</param>
/// <returns>Reflected vector</returns>
template<size_t l, class t>
constexpr vec<l, t> reflect(const vec<l, t>& d, const vec<l, t>& n) {
	return d - n * 2 * dot(d, n);
}

#undef VECTOR_OPERATOR
#undef VECTOR_S_OPERATOR
#undef VECTOR_ELEMENT
#undef VECTOR_ELEMENTS
#undef VECTOR
#undef VECTOR_C_OPERATOR
#undef ELE_V4
#undef ELE_V4_
#undef ELE_V4__
#undef ELE_V4___
#undef ELE_V3
#undef ELE_V3_
#undef ELE_V3__
#undef ELE_V2
#undef ELE_V2_
#undef STANDARD_SUB_VECTORS
#undef VECTOR_ELEMENT
#undef FUNC_CALL
#undef ELE_V