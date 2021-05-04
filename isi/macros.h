#pragma once

#define NARGS_SEQ(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define NARGS_MSVC_WORKAROUND(x) NARGS_SEQ x
#define NARGS(...) NARGS_MSVC_WORKAROUND((__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1))

#define PRIMITIVE_CAT(x, y) x##y
#define CAT_MSVC_WORKAROUND(x) PRIMITIVE_CAT x
#define CAT(x, y) CAT_MSVC_WORKAROUND((x, y))

#define NO_SPACES(...) \
  APPLY_MSVC_WORKAROUND(CAT(NO_SPACES_, NARGS(__VA_ARGS__)), (__VA_ARGS__))
#define NO_SPACES_1(x1) x1
#define NO_SPACES_2(x1, x2) x1##x2
#define NO_SPACES_3(x1, x2, x3) x1##x2##x3
#define NO_SPACES_4(x1, x2, x3, x4) x1##x2##x3##x4
#define NO_SPACES_5(x1, x2, x3, x4, x5) x1##x2##x3##x4##x5

#define APPLY(macro, ...) \
  APPLY_MSVC_WORKAROUND(CAT(APPLY_, NARGS(__VA_ARGS__)), (macro, __VA_ARGS__))
#define APPLY_MSVC_WORKAROUND(m, x) m x

#define APPLY_1(m, x1) m(x1)
#define APPLY_2(m, x1, x2) m(x1), m(x2)
#define APPLY_3(m, x1, x2, x3) m(x1), m(x2), m(x3)
#define APPLY_4(m, x1, x2, x3, x4) m(x1), m(x2), m(x3), m(x4)
#define APPLY_5(m, x1, x2, x3, x4, x5) m(x1), m(x2), m(x3), m(x4), m(x5)
#define APPLY_6(m, x1, x2, x3, x4, x5, x6) \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6)
#define APPLY_7(m, x1, x2, x3, x4, x5, x6, x7) \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7)
#define APPLY_8(m, x1, x2, x3, x4, x5, x6, x7, x8) \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8)
#define APPLY_9(m, x1, x2, x3, x4, x5, x6, x7, x8, x9) \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9)
#define APPLY_10(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10)
#define APPLY_11(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), m(x11)
#define APPLY_12(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12)   \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10), \
      m(x11), m(x12)
#define APPLY_13(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10),    \
      m(x11), m(x12), m(x13)
#define APPLY_14(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, \
                 x14)                                                       \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10),    \
      m(x11), m(x12), m(x13), m(x14)
#define APPLY_15(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, \
                 x14, x15)                                                  \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10),    \
      m(x11), m(x12), m(x13), m(x14), m(x15)
#define APPLY_16(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, \
                 x14, x15, x16)                                             \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10),    \
      m(x11), m(x12), m(x13), m(x14), m(x15), m(x16)
#define APPLY_17(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, \
                 x14, x15, x16, x17)                                        \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10),    \
      m(x11), m(x12), m(x13), m(x14), m(x15), m(x16), m(x17)
#define APPLY_18(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, \
                 x14, x15, x16, x17, x18)                                   \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10),    \
      m(x11), m(x12), m(x13), m(x14), m(x15), m(x16), m(x17), m(x18)
#define APPLY_19(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, \
                 x14, x15, x16, x17, x18, x19)                              \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10),    \
      m(x11), m(x12), m(x13), m(x14), m(x15), m(x16), m(x17), m(x18), m(x19)

#define APPLY_20(m, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13,   \
                 x14, x15, x16, x17, x18, x19, x20)                           \
  m(x1), m(x2), m(x3), m(x4), m(x5), m(x6), m(x7), m(x8), m(x9), m(x10),      \
      m(x11), m(x12), m(x13), m(x14), m(x15), m(x16), m(x17), m(x18), m(x19), \
      m(x20)

#define LOCAL_COUNTER __COUNTER__ - COUNTER_BASE - 1
#define P(...) __VA_ARGS__