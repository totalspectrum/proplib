#include <math.h>
#include "math_private.h"
#include <compiler.h>

int
__isinfl(long double x)
{
  return __builtin_isinfl(x);
}

int
__isinff(float x)
{
  return __builtin_isinff(x);
}

#if defined(__SHORT_DOUBLES_IMPL)

__strong_alias(__isinf, __isinff);

#elif LDBL_MANT_DIG == 53

__strong_alias(__isinf, __isinfl);

#else

int
__isinf(double x)
{
  return __builtin_isinf(x);
}

#endif
