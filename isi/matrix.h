#pragma once
#include "vector.h"

template<size_t w, size_t h, Floating t = DEFAULT_FLOATING>
struct matrix {
private:
	// Store matrix data in a vector of vectors
	vec<h, vec<w, t>> data;

public:
	constexpr matrix() : data() { }
	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="other"></param>
	constexpr matrix(const matrix<h, w, t>& other) : data(other.data) {}
	/// <summary>
	/// Create matrix from individual elements. Element count must equal width * height.
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	template <typename... Args>
	constexpr matrix(Args... args) requires (sizeof...(Args) == w * h) {
		size_t loop = 0;
		((data[loop / w][(loop++) % w] = static_cast<t>(args)), ...);
	}

	/// <summary>
	/// Create matrix from row vectors.
	/// </summary>
	template <typename... Args>
	constexpr matrix(Args... args) requires (sizeof...(Args) == h) : data(args...) { }


	/// <summary>
	/// Get a row.
	/// </summary>
	constexpr vec<w, t> row(size_t index) const {
		if (index >= h) throw;
		return data[index];
	}

	/// <summary>
	/// Get a column.
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	constexpr vec<h, t> col(size_t index) const {
		if (index >= w) throw;
		vec<h, t> result;
		for (size_t i = 0; i < h; i++) {
			result[i] = data[i][index];
		}
		return result;
	}
	// Returns a vector of the row at the index
	constexpr vec<w, t>& operator[](size_t index) {
		return data[index];
	}
	/// <summary>
	/// matrix multiplication
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	template<size_t _w>
	constexpr matrix<_w, h, t> operator*(const matrix<_w, w, t>& other) const {
		matrix<_w, h, t> result;

		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < _w; x++) {
				result[y][x] = dot(col(y), other.row(x));
			}
		}
		return result;
	}
	/// <summary>
	/// Multiply vector by this matrix.
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	constexpr vec<h, t> operator*(const vec<h, t>& other) const {
		vec<h, t> result;
		for (size_t y = 0; y < h; y++)
		{
			result[y] = dot(col(y), other);
		}
		return result;
	}
	/// <summary>
	/// Scale matrix
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	constexpr matrix<w, h, t> operator*(const t& other) const {
		matrix<w, h, t> result;

		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < w; x++) {
				result[y][x] = data[y][x] * other;
			}
		}
		return result;
	}

	constexpr matrix<w, h, t>& operator*=(const t& other) {

		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < w; x++) {
				data[y][x] *= other;
			}
		}
		return *this;
	}

	/// <summary>
	/// Get sub matrix, where you exclude the row at _y and the column at _x.
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	constexpr matrix<w - 1, h - 1, t> get_sub(size_t _x, size_t _y) const requires (w > 1 && h > 1) {
		matrix<w - 1, w - 1, t> sub;
		size_t loop = 0;
		for (size_t y = 0; y < h; y++)
		{
			if (y == _y) continue;
			for (size_t x = 0; x < w; x++)
			{
				if (x == _x) continue;
				sub[loop / (w - 1)][loop++ % (w - 1)] = data[y][x];
			}
		}
		return sub;
	}

	/// <summary>
	/// Set a reference to a sub.
	/// </summary>
	/// <param name="sub"></param>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	constexpr void get_sub(matrix<w - 1, h - 1, t>& sub, size_t _x, size_t _y) const requires (w > 1 && h > 1) {
		size_t loop = 0;
		for (size_t y = 0; y < h; y++)
		{
			if (y == _y) continue;
			for (size_t x = 0; x < w; x++)
			{
				if (x == _x) continue;
				sub[loop / (w - 1)][loop++ % (w - 1)] = data[y][x];
			}
		}
	}

	/// <summary>
	/// Get determinant of this matrix
	/// </summary>
	/// <returns></returns>
	constexpr t determinant() const requires (w == h && w == 1) {
		return data[0][0];
	}
	/// <summary>
	/// Get determinant of this matrix
	/// </summary>
	/// <returns></returns>
	constexpr t determinant() const requires (w == h && w == 2) {
		return data[0][0] * data[1][1] - data[1][0] * data[0][1];
	}

	/// <summary>
	/// Get determinant of this matrix
	/// </summary>
	/// <returns></returns>
	constexpr t determinant() const requires (w == h && w >= 3) {
		matrix<w - 1, w - 1, t> sub;
		t d{ 0 };
		t sign{ 1 };
		for (size_t x = 0; x < w; x++)
		{
			get_sub(sub, x, 0);
			d += sign * data[0][x] * sub.determinant();
			sign = -sign;
		}
		return d;
	}

	/// <summary>
	/// Get a transposed version of this matrix.
	/// </summary>
	/// <returns></returns>
	constexpr matrix<h, w, t> transposed() const {
		matrix<h, w, t> result;
		for (size_t y = 0; y < h; y++)
		{
			for (size_t x = 0; x < w; x++)
			{
				result[x][y] = data[y][x];
			}
		}
		return result;
	}

	/// <summary>
	/// Get the cofactor of this matrix.
	/// </summary>
	/// <returns></returns>
	constexpr matrix<w, h, t> cofactor() const requires (w == h && w > 1) {
		matrix<w, h, t> result;
		matrix<w - 1, h - 1, t> sub;

		for (size_t y = 0; y < h; y++)
		{
			for (size_t x = 0; x < w; x++)
			{
				get_sub(sub, x, y);
				result[y][x] = ((!((y + x) % 2)) * 2 - 1) * sub.determinant();
			}
		}
		return result;
	}

	/// <summary>
	/// Get the identity matrix.
	/// </summary>
	/// <returns></returns>
	static constexpr matrix<w, h, t> identity() {
		constexpr size_t max = min(w, h);
		matrix<w, h, t> result;
		for (size_t i = 0; i < max; i++)
		{
			result[i][i] = t{ 1 };
		}
		return result;
	}

	/// <summary>
	/// Inverse of 2x2 matrix.
	/// </summary>
	/// <returns></returns>
	constexpr matrix<2, 2, t> inversed() const requires (w == 2 && h == 2) {
		t d = determinant();
		if (d == t{ 0 }) throw;
		d = 1 / d;
		return matrix<2, 2, t>(data[1][1] * d, -data[0][1] * d,
			-data[1][0] * d, data[0][0] * d);
	}

	/// <summary>
	/// Inverse of 3x3 matrix
	/// </summary>
	/// <returns></returns>
	constexpr matrix<3, 3, t> inversed() const requires (w == 3 && h == 3) {
		t d = determinant();
		if (d == t{ 0 }) throw;
		d = 1 / d;
		return matrix<3, 3, t>((data[1][1] * data[2][2] - data[1][2] * data[2][1]) * d, (data[0][2] * data[2][1] - data[0][1] * data[2][2]) * d, (data[0][1] * data[1][2] - data[0][2] * data[1][1]) * d,
			(data[1][2] * data[2][0] - data[1][0] * data[2][2]) * d, (data[0][0] * data[2][2] - data[0][2] * data[2][0]) * d, (data[0][2] * data[1][0] - data[0][0] * data[1][2]) * d,
			(data[1][0] * data[2][1] - data[1][1] * data[2][0]) * d, (data[0][1] * data[2][0] - data[0][0] * data[2][1]) * d, (data[0][0] * data[1][1] - data[0][1] * data[1][0]) * d);
	}

	// Function to invert a square matrix of any size.
	// Inverting a matrix scales O(2^n) where n is the number of rows or columns so not very efficient for large matrices.
	// According to my tests on my craptop an 8x8 matrix takes ~6ms to inverse.
	/// <summary>
	/// Inverse of any size matrix. Is slow for big matrices
	/// </summary>
	/// <returns></returns>
	constexpr matrix<w, h, t> inversed() const requires (w == h && w > 3) {
		t  d = determinant();

		if (d == t{ 0 }) throw;

		d = 1 / d;

		matrix<w, h, t> result = cofactor().transposed();

		for (size_t y = 0; y < h; y++)
		{
			for (size_t x = 0; x < w; x++)
			{
				result[y][x] *= d;
			}
		}
		return result;
	}

	/// <summary>
	/// Add matrices
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	constexpr matrix<w, h, t> operator+(const matrix<w, h, t>& other) const {
		matrix<w, h, t> result;

		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < w; x++) {
				result[y][x] = data[y][x] + other[y][x];
			}
		}
		return result;
	}


	constexpr matrix<w, h, t> operator-(const matrix<w, h, t>& other) const {
		matrix<w, h, t> result;

		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < w; x++) {
				result[y][x] = data[y][x] - other[y][x];
			}
		}
		return result;
	}

	constexpr matrix<w, h, t>& operator+=(const matrix<w, h, t>& other) {
		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < w; x++) {
				data[y][x] += other[y][x];
			}
		}
		return *this;
	}

	constexpr matrix<w, h, t>& operator-=(const matrix<w, h, t>& other) {
		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < w; x++) {
				data[y][x] -= other[y][x];
			}
		}
		return *this;
	}
	/// <summary>
	/// Print matrix. Does not look nice right now. 
	/// To make it look nicer one could first store all the strings of the elements and make sure each element takes the same amount of space in the final string.
	/// </summary>
	/// <param name="os"></param>
	/// <param name="mat"></param>
	/// <returns></returns>
	friend std::ostream& operator<<(std::ostream& os, const matrix<w, h, t>& mat) {

		for (size_t y = 0; y < h; y++)
		{
			if (y) os << std::endl;
			os << '|';
			for (size_t x = 0; x < w; x++) {
				os << ' ' << mat.data[y][x] << ' ';
			}
			os << '|';
		}
		return os;
	}
};
/// <summary>
/// Template specializeation for matrices with width 0, to not create problems.
/// </summary>
template<size_t h, Floating t>
struct matrix<0, h, t> {

};
/// <summary>
/// Template specializeation for matrices with height 0, to not create problems.
/// </summary>
template<size_t w, Floating t>
struct matrix<w, 0, t> {

};

/// <summary>
/// Convertion from quaternion to rotation matrix.
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
template<Floating t = DEFAULT_FLOATING>
constexpr matrix<3, 3, t> quat2rotm(const quaternion<t>& q) {
	return matrix<3, 3, t>(t{ 2.0 } *(q.x * q.x + q.y * q.y) - t{ 1.0 },
		t{ 2.0 } *(q.y * q.z - q.x * q.w),
		t{ 2.0 } *(q.y * q.w + q.x * q.z),r det har
		t{ 2.0 } *(q.y * q.z + q.x * q.w),
		t{ 2.0 } *(q.x * q.x + q.z * q.z) - t{ 1.0 },
		t{ 2.0 } *(q.z * q.w - q.x * q.y),
		t{ 2.0 } *(q.y * q.w - q.x * q.z),
		t{ 2.0 } *(q.z * q.w + q.x * q.y),
		t{ 2.0 } *(q.x * q.x + q.w * q.w) - t{ 1.0 });
}
/// <summary>
/// Get the translation of a transform matrix.
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
template<Floating t = DEFAULT_FLOATING>
constexpr v3<t> transfm_center(const matrix<4, 4, t>& m) {
	return vec<3, t>(m[0][3], m[1][3], m[2][3]);
}

/// <summary>
/// Convertion from rotation matrix to quaternion.
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
template<Floating t = DEFAULT_FLOATING>
constexpr quaternion<t> rotm2quat(const matrix<3, 3, t>& m) {
	quaternion result;
	result.w = std::sqrt(t{ 1.0 } + m[0][0] + m[1][1] + m[2][2]) / t{ 2.0 };
	t w4 = (t{ 4.0 } *result.w);
	result.x = (m[2][1] - m[1][2]) / w4;
	result.y = (m[0][2] - m[2][0]) / w4;
	result.z = (m[1][0] - m[0][1]) / w4;
	return result;
}


#define MATRIX(w, h) using matrix##w##x##h = matrix<w, h>; template<typename t = DEFAULT_FLOATING> using matrix##w##x##h##_ = matrix<w, h, t>

// Define default matrices.
MATRIX(2, 2);
MATRIX(2, 3);
MATRIX(3, 2);
MATRIX(3, 3);
MATRIX(2, 4);
MATRIX(4, 2);
MATRIX(3, 4);
MATRIX(4, 3);
MATRIX(4, 4);

#undef MATRIX