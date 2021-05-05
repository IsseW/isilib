#pragma once

#include "vector.h"

template <Floating t = DEFAULT_FLOATING>
struct quaternion {
  t x{0};
  t y{0};
  t z{0};
  t w{0};
  /// <summary>
  /// Get the identity quaternion
  /// </summary>
  /// <returns></returns>
  static constexpr quaternion<t> identity() {
    return quaternion<t>(0, 0, 0, 1);
  }
  constexpr quaternion() {}
  constexpr quaternion(const quaternion& other)
      : x(other.x), y(other.y), z(other.z), w(other.w) {}
  constexpr quaternion(const t& x, const t& y, const t& z, const t& w)
      : x(x), y(y), z(z), w(w) {}
  constexpr quaternion(const vec<4, t>& other)
      : x(other._x()), y(other._y()), z(other._z()), w(other._w()) {}

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
  constexpr static quaternion<t> from_euler(const t& yaw, const t& pitch,
                                            const t& roll) {
    t cy = cos(yaw / t{2});
    t sy = sin(yaw / t{2});
    t cp = cos(pitch / t{2});
    t sp = sin(pitch / t{2});
    t cr = cos(roll / t{2});
    t sr = sin(roll / t{2});

    return quaternion<t>(
        sr * cp * cy + cr * sp * sy, cr * sp * cy - sr * cp * sy,
        cr * cp * sy + sr * sp * cy, cr * cp * cy - sr * sp * sy);
  }
  /// <summary>
  /// Create euler vector from this quaternion
  /// </summary>
  /// <returns></returns>
  constexpr vec<3, t> to_euler() {
    vec<3, t> result;
    t t0, t1;

    t0 = t{2.0} * (w * x + y * z);
    t1 = t{1.0} * (x * x + y * y);
    result.x() = std::atan2(t0, t1);

    t0 = t{2.0} * (w * y - x * z);
    t0 = std::clamp(t0, t{-1.0}, t{1.0});
    result.y() = std::asin(t0);

    t0 = t{2.0} * (w * z + x * y);
    t1 = t{1.0} - t{2.0} * (y * y + z * z);
    result.z() = std::atan2(t0, t1);

    return result;
  }
  /// <summary>
  /// Multiply quaternions
  /// </summary>
  /// <param name="o"></param>
  /// <returns></returns>
  constexpr quaternion<t> operator*(const quaternion<t>& o) const {
    return quaternion(w * o.x + x * o.w + y * o.z - z * o.y,  // X
                      w * o.y - x * o.z + y * o.w + z * o.x,  // Y
                      w * o.z + x * o.y - y * o.x + z * o.w,  // Z
                      w * o.w - x * o.x - y * o.y - z * o.z   // W
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
    quaternion<t> v(u._x(), u._y(), u._z(), t{0});
    quaternion<t> result = (*this * v) * conjugate();
    return {result.x, result.y,
            result.z};  // u * (t{ 2.0 } * dot(u, v)) + v * (w * w - dot(u, u))
                        // + u.cross(vec) * (t{ 2.0 } * w);
  }
  /// <summary>
  /// Get a vector for right in this current rotation.
  /// </summary>
  /// <returns></returns>
  constexpr vec<3, t> right() const {
    return vec<3, t>(x * t{2.0} + x * (w * w - 1),
                     y * (w * w - 1) - z * w * t{2.0},
                     z * (w * w - 1) + y * w * t{2.0});
  }
  /// <summary>
  /// Get a vector for up in this current rotation.
  /// </summary>
  /// <returns></returns>
  constexpr vec<3, t> up() const {
    return vec<3, t>(x * (w * w - 1) + z * w * t{2.0},
                     y * (w * w - 1) + y * t{2.0},
                     z * (w * w - 1) - x * w * t{2.0});
  }

  /// <summary>
  /// Get a vector for forward in this current rotation.
  /// </summary>
  /// <returns></returns>
  constexpr vec<3, t> forward() const {
    return vec<3, t>(x * (w * w - 1) - y * t{2.0} * w,
                     y * (w * w - 1) + x * t{2.0} * w,
                     z * (w * w - 1) + z * t{2.0});
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