#ifndef DECIMAL_H
#define DECIMAL_H

#include <math.h>

#define USE_DOUBLE false
#define USE_FLOAT false

#if USE_DOUBLE
    typedef double decimal;
    #define sin(x) sin(x)
    #define cos(x) cos(x)
    #define acos(x) acos(x)
    #define atan2(y, x) atan2(y, x)
    #define sqrt(x) sqrt(x)
    #define abs(x) fabs(x)
    #define round(x) round(x)
    #define exp(x) exp(x)
    #define strto(x) strtod(x)
    #define FORMAT_SPECIFIER "lf"
#elif USE_FLOAT
    typedef float decimal;
    #define sin(x) sinf(x)
    #define cos(x) cosf(x)
    #define acos(x) acosf(x)
    #define atan2(y, x) atan2f(y, x)
    #define sqrt(x) sqrtf(x)
    #define abs(x) fabsf(x)
    #define round(x) roundf(x)
    #define exp(x) expf(x)
    #define strto(x) strtof(x)
    #define FORMAT_SPECIFIER "f"
#else
    typedef long double decimal;
    #define sin(x) sinl(x)
    #define cos(x) cosl(x)
    #define acos(x) acosl(x)
    #define atan2(y, x) atan2l(y, x)
    #define sqrt(x) sqrtl(x)
    #define abs(x) fabsl(x)
    #define round(x) roundl(x)
    #define exp(x) expl(x)
    #define strto(x) strtold(x)
    #define FORMAT_SPECIFIER "Lf"
#endif

#endif // DECIMAL_H