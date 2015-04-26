/*
 * @strtod.c
 * Convert string to double.
 *
 * Copyright (c) 2011 Parallax, Inc.
 * Copyright (c) 2015 Total Spectrum Software Inc.
 * Written by Eric R. Smith, Total Spectrum Software Inc.
 * MIT licensed (see terms at end of file)
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <compiler.h>
#include <errno.h>
#include <stdint.h>

// calculate a*b^n with extra precision
extern long double _intpowfx(long double a, long double b, int n, void *);
extern long double _intpowix(uint64_t a, uint64_t b, int n, void *);

long double
strtold(const char *str, char **endptr)
{
  long double v = 0.0;
  int base = 10;
  int hex = 0;
  int minus = 0;
  int minuse = 0;
  int c;
  int exp = 0;
  int digits = 0;
  int maxdigits;
  uint64_t ai = 0;
  int moredigits = 0;

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
    base = 16;
    str++;
    c = toupper(*str++);
  }

  if (hex) {
    maxdigits = 13;
  } else {
    maxdigits = 17;
  }

  /* get up to "maxdigits" digits */
  exp = 0;
  v = 0.0;

  while ( isdigit(c) 
          || (hex && isxdigit(c) && (c = c - 'A' + 10 + '0') ) )
  {
      if (digits <  maxdigits) {
          ai = ai * base + (c - '0');
          digits++;
      } else {
          exp++;
          // round if this is the first extra digit seen
          if (!moredigits) {
              if ( (c - '0') >= base/2 ) ai++;
          }
          moredigits = 1;
      }
      c = toupper(*str++);
  }
  if (c == '.') {
      c = toupper(*str++);
      while (c == '0' && ai == 0) {
          --exp;
          c = toupper(*str++);
      }
      while ( isdigit(c) 
              || (hex && isxdigit(c) && (c = c - 'A' + 10 + '0') ) )
      {
          if (digits < maxdigits) {
              ai = ai * base + (c - '0');
              digits++;
              --exp;
          } else {
              // round if this is the first extra digit seen
              if (!moredigits) {
                  if ( (c - '0') >= base/2 ) ai++;
              }
              moredigits = 1;
          }
          c = toupper(*str++);
      }
  }
  if (hex) {
    // convert exponent to binary
    exp *= 4;
  }
  if (c == 'E' || (hex && c == 'P') ) {
      int tmpe = 0;
      c = *str++;
      if (c == '-') {
	minuse = 1;
	c = *str++;
      } else if (c == '+') {
	c = *str++;
      }
      while (isdigit(c)) {
	  tmpe = 10*tmpe + (c-'0');
	  c = *str++;
      }
      if (minuse) {
	  tmpe = -tmpe;
      }
      exp += tmpe;
  }

  v = _intpowix(ai, hex ? 2 : 10, exp, NULL);

  if (v == HUGE_VALL) {
    errno = ERANGE;
  }
 done:
  if (endptr)
    *endptr = (char *)(str-1);
  if (minus) {
      v = copysignl(v, -1.0L); // make sure nan is signed properly
  }
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
