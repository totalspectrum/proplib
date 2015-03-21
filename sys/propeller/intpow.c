#include <compiler.h>

//
// raise a double to an integer power
// this is used in sprintf and strtod, so it should
// be as accurate as possible (particularly for
// powers of 10)
//

#if __GNUC___ >= 5
//#error do not need this
// in gcc5 we have an assembly version of this in libgcc,
// but for older propgcc we need it here
#else

//
// calculate a * b^n
//
//
// we do the b^n calculation by using powers of 2;
// e.g. b^6 = b^4 * b^2
// this is convenient for an integer n, since the
// binary representation gives us which powers we
// need
//
// in general for the things we're interested in
// (powers of 10 particularly) calculating with
// positive powers is more accurate, so for negative
// powers we calculate the inverse and divide
//
// powers of 10 are particularly important, so handle those
// with a table
// up to 1e16 the calculations are exact
#define FIRST_POW10 5 // 2^5=32
#define LAST_POW10  8 // 2^8 = 256

static long double pow10[] = {
    1.0e32L, 1.0e64L, 1.0e128L, 1.0e256L
};

long double _intpowdf(long double a, long double b, int n)
{
    int sign = 0;
    long double power = b;
    long double result = 1.0L;
    int ispow10 = (b == 10.0L);
    int i;

    if (n < 0) {
        sign = 1;
        n = -n;
        if (n < 0) {
            // handle the case where n == 0x80000000
            // if a is NaN or Inf we need to process it
            // correctly; similarly for determining
            // whether the result is 0.0 or -0.0; the
            // statement below will handle all those cases
            return a * 0.0;
        }
    }
    i = 0;
    while ( n > 0 ) {
        if (n & 1) {
            result = result * power;
        }
        n = n>>1;
        i = i+1; 
        if (ispow10 && (i>=FIRST_POW10) && (i<=LAST_POW10)) {
            power = pow10[i-FIRST_POW10];
        } else {
            power = power*power;
        }
    }

    if (sign) {
        return a / result;
    }
    return a * result;
}

__strong_alias(_intpow, _intpowdf);

#endif
