#pragma once

#include <exception>
#include <string>
#include "const_math.h"

namespace types {
	typedef unsigned char u8;
	typedef signed char i8;
	typedef signed short i16;
	typedef unsigned short u16;
	typedef signed int i32;
	typedef unsigned int u32;
	typedef signed long long int i64;
	typedef unsigned long long int u64;
	typedef float f32;
	typedef double f64;
	typedef std::string str;

	struct q8 {
		constexpr q8() : data(0) {}
		constexpr q8(u8 data) : data(data) {}

		constexpr q8 operator+(const q8& other) { return q8(data + other.data); }
		constexpr q8 operator-(const q8& other) { return q8(data - other.data); }
		constexpr q8 operator*(const q8& other) {
			return q8(((((u16)data) << 8) / other.data) >> 8);
		}
		constexpr q8 operator/(const q8& other) {
			return q8((u8)(((u16)data * (u16)other.data + 0xFF) >> 8));
		}
		constexpr u8 operator*(const u8& other) {
			return (u8)(((u16)data * (u16)other + 0xFF) >> 8);
		}
		constexpr u16 operator*(const u16& other) {
			return (data * other + 0xFF) >> 8;
		}
		constexpr u32 operator*(const u32& other) {
			return (data * other + 0xFF) >> 8;
		}
		constexpr u64 operator*(const u64& other) {
			return (data * other + 0xFF) >> 8;
		}
		constexpr i8 operator*(const i8& other) {
			return (i8)(((i16)data * (i16)other) >> 8);
		}
		constexpr i16 operator*(const i16& other) { return (data * other) >> 8; }
		constexpr i32 operator*(const i32& other) { return (data * other) >> 8; }
		constexpr i64 operator*(const i64& other) { return (data * other) >> 8; }
		constexpr f32 operator*(const f32& other) { return data * other / 255.0f; }
		constexpr f64 operator*(const f64& other) { return data * other / 255.0; }

		constexpr u8 get_raw() const { return data; }

	private:
		u8 data;
	};
	// d should be between 0.0 and 1.0
	constexpr static q8 operator""_q(const long double d) {
		return q8((u8)(d * 255));
	};

}  // namespace types

namespace consts {
	namespace q8 {
		constexpr static types::q8 one = types::q8(255);
		constexpr static types::q8 half = types::q8(128);
		constexpr static types::q8 quarter = types::q8(64);
		constexpr static types::q8 third = types::q8(85);
		constexpr static types::q8 zero = types::q8(0);
	}  // namespace q8
	namespace f32 {
		constexpr static types::f32 pi = 3.141592653589f;
		constexpr static types::f32 inf = HUGE_VALF;
	}
	namespace f64 {
		constexpr static types::f64 pi = 3.1415926535897932384626433832795;
		constexpr static types::f64 inf = HUGE_VAL;
	}
}  // namespace consts

using namespace types;

constexpr static long double operator""_deg(const long double d) {
	return d * consts::f64::pi / 180.0;
};