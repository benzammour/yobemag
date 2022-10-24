#ifndef YOBEMAG_ATTRIBUTES_H
#define YOBEMAG_ATTRIBUTES_H

#define ATTR_NORETURN __attribute__((__noreturn__))

#define ATTR_FORMAT3 __attribute__((format(printf, 3, 4)))

#define ATTR_FORMAT4 __attribute__((format(printf, 4, 5)))

#define ATTR_ALWAYS_INLINE __attribute__((always_inline))

#define ATTR_PURE __attribute__((pure))

#endif //YOBEMAG_ATTRIBUTES_H
