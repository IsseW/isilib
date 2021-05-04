#pragma once
#include "vector.h"

#include "primitives.h"
vec<2, u16> num(512, 512);
u16 numOctaves = 7;
f64 persistence = 0.5;

#define maxPrimeIndex 10
u32 primeIndex = 0;

u32 primes[maxPrimeIndex][3] = {
  { 995615039, 600173719, 701464987 },
  { 831731269, 162318869, 136250887 },
  { 174329291, 946737083, 245679977 },
  { 362489573, 795918041, 350777237 },
  { 457025711, 880830799, 909678923 },
  { 787070341, 177340217, 593320781 },
  { 405493717, 291031019, 391950901 },
  { 458904767, 676625681, 424452397 },
  { 531736441, 939683957, 810651871 },
  { 997169939, 842027887, 423882827 }
};
template<Floating T = DEFAULT_FLOATING>
constexpr T Noise(int i, int x, int y) {
    i32 n = x + y * 57;
    n = (n << 13) ^ n;
    i32 a = primes[i][0], b = primes[i][1], c = primes[i][2];
    i32 t = (n * (n * n * a + b) + c) & 0x7fffffff;
    return T{ 1.0 } - static_cast<T>(t) / T{ 1073741824.0 };
}

template<Floating T = DEFAULT_FLOATING>
constexpr T SmoothedNoise(i32 i, i32 x, i32 y) {
    constexpr T one = T{ 1.0 };
    T corners = (Noise(i, x - one, y - one) + Noise(i, x + one, y - one) +
        Noise(i, x - one, y + one) + Noise(i, x + one, y + one)) / T{ 16.0 },
        sides = (Noise(i, x - one, y) + Noise(i, x + 1, y) + Noise(i, x, y - one) +
            Noise(i, x, y + one)) / T{ 8.0 },
        center = Noise(i, x, y) / T{ 4.0 };
    return corners + sides + center;
}

template<Floating T = DEFAULT_FLOATING>
constexpr T Interpolate(T a, T b, T x) {  // cosine interpolation
    T ft = x * T{ consts::f64::pi },
        f = (1 - std::cos(ft)) * T{ 0.5 };
    return  a * (1 - f) + b * f;
}

template<Floating T = DEFAULT_FLOATING>
T InterpolatedNoise(i32 i, T x, T y) {
    i32 integer_X = x;
    T fractional_X = x - integer_X;
    i32 integer_Y = y;
    T fractional_Y = y - integer_Y;

    T v1 = SmoothedNoise(i, integer_X, integer_Y),
        v2 = SmoothedNoise(i, integer_X + 1, integer_Y),
        v3 = SmoothedNoise(i, integer_X, integer_Y + 1),
        v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1),
        i1 = Interpolate(v1, v2, fractional_X),
        i2 = Interpolate(v3, v4, fractional_X);
    return Interpolate(i1, i2, fractional_Y);
}

template<Floating T = DEFAULT_FLOATING>
T Simplex(T x, T y) {
    T total = T{ 0 },
        frequency = std::pow(T{ 2 }, numOctaves),
        amplitude = T{ 1 };
    for (u16 i = 0; i < numOctaves; ++i) {
        frequency /= T{ 2 };
        amplitude *= persistence;
        total += InterpolatedNoise((primeIndex + i) % maxPrimeIndex,
            x / frequency, y / frequency) * amplitude;
    }
    return total / frequency;
}