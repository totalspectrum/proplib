/*
 * @strtod.c
 * Convert string to double.
 *
 * Copyright (c) 2011 Parallax, Inc.
 * Written by Eric R. Smith, Total Spectrum Software Inc.
 * MIT licensed (see terms at end of file)
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <compiler.h>
#include <errno.h>

#if 0
/*
 * calculate v * 10^n
 *
 * e.g. for v * 10^13 (where 13 = 0xD == 0b01101) 
 *
 * calc = 10.0 * 10^4 * 10^8
 */
static long double _tens[] = {
  1.0e2L, 1.0e4L, 1.0e8L, 1.0e16L,
  1.0e32L, 1.0e64L, 1.0e128L, 1.0e256L
};
static long double _tenths[] = {
  1.0e-2L, 1.0e-4L, 1.0e-8L, 1.0e-16L,
  1.0e-32L, 1.0e-64L, 1.0e-128L, 1.0e-256L
};

long double
__mul_pow10(long double v, int n)
{
  long double powten;
  long double calc = 1.0;
  int idx = 0;
  long double *table;

  if (n < 0)
    {
      powten = 0.1;
      table = _tenths;
      n = -n;
    }
  else
  {
      powten = 10.0;
      table = _tens;
  }
  while (n > 0)
    {
        if (n & 1)
	{
	    calc = calc * powten;
	}
        if (idx < 8) {
            powten = table[idx];
            idx++;
        } else {
            powten = powten * powten;
        }
        n = n>>1;
    }
  v = v * calc;
  return v;
}
#else
extern long double _intpowdf(long double a, long double b, int n);
#define mypow _intpowdf
#endif

long double
strtold(const char *str, char **endptr)
{
  long double v = 0.0;
  long double base = 10.0;
  int hex = 0;
  int minus = 0;
  int minuse = 0;
  int c;
  int exp = 0;
  int expbump;
  int digits;

  while (isspace(*str))
    str++;
  if (*str == '-')
    {
      minus = 1;
      str++;
    }
  else if (*str == '+')
    str++;

  c = toupper(*str++);
  if (c == 'I') {
    if (toupper(str[0]) == 'N' && toupper(str[1]) == 'F')
      {
	str += 3;
	v = HUGE_VALL;
	if (minus) v = -v;
	goto done;
      }
  } else if (c == 'N') {
    if (toupper(str[0]) == 'A' && toupper(str[1]) == 'N')
      {
	str += 2;
	if (*str == '(')
	  {
	    /* we actually should parse this, but it's all implementation
	       defined anyway */
	    do {
	      c = *str++;
	    } while (c != ')');
	    str++;
	  }
	v = _NANL;
	if (minus) v = -v;
	goto done;
      }
  }

  if (c == '0' && toupper(*str) == 'X') {
    hex = 1;
    base = 16.0;
    str++;
    c = toupper(*str++);
  }

  if (hex) {
    digits = 14;
  } else {
    digits = 18;
  }

  /* get up to "digits" digits */
  exp = 0;
  expbump = 0;
  v = 0.0;

  while (digits > 0
	 && (isdigit(c) 
	     || (hex && isxdigit(c) && (c = c - 'A' + 10 + '0'))
	     || (c == '.' && expbump == 0)))
    {
      if (c == '.') {
	expbump = -1;
      } else {
	v = base * v + (c - '0');
	exp += expbump;
	--digits;
      }
      c = toupper(*str++);
    }
  expbump++;
  // skip any superfluous digits 
  while (isdigit(c) || (hex && isxdigit(c)) || (c == '.' && expbump == 1))
    {
      if (c == '.')
	expbump = 0;
      else
	exp += expbump;
      c = toupper(*str++);
    }

  if (hex) {
    // convert exponent to binary
    exp *= 4;
  }
  if (c == 'E' || c == 'P') 
    {
      int tmpe = 0;
      c = *str++;
      if (c == '-') {
	minuse = 1;
	c = *str++;
      } else if (c == '+') {
	c = *str++;
      }
      while (isdigit(c))
	{
	  tmpe = 10*tmpe + (c-'0');
	  c = *str++;
	}
      if (minuse)
	{
	  tmpe = -tmpe;
	}
      exp += tmpe;
    }

#if 0
  if (hex) {
      v = ldexp(v, exp);
  } else {
      v = __mul_pow10(v, exp);
  }
#else
  v = mypow(v, hex ? 2.0L : 10.0L, exp);
#endif
  if (v == HUGE_VALL)
    errno = ERANGE;

 done:
  if (endptr)
    *endptr = (char *)(str-1);
  if (minus)
    v = -v;
  return v;
}

#if defined(__PROPELLER_64BIT_DOUBLES__)
double strtod(const char *str, char **endptr) __attribute__((alias("strtold")));
#endif

/* +--------------------------------------------------------------------
 * ¦  TERMS OF USE: MIT License
 * +--------------------------------------------------------------------
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * +--------------------------------------------------------------------
 */
