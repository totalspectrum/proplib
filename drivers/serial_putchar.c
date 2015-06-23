/*
 * simple serial transmission routine
 * simple serial driver
 * Copyright (c) Parallax Inc. 2011
 * Modifications Copyright (c) 2015 Total Spectrum Software Inc.
 * MIT Licensed (see end of file)
 */
#include <propeller.h>
#include <sys/serial.h>

/* globals that the loader may change; these represent the default
 * pins to use
 */
extern unsigned int _rxpin;
extern unsigned int _txpin;
extern unsigned int _baud;

/*
 * We need _serial_txbyte to always be fcached so that the timing is
 * OK.
 */
__attribute__((fcache))
int _serial_tx(int c, unsigned int txmask, unsigned int bitcycles)
{
  unsigned int waitcycles;
  int i, value;

  /* set output */
  _OUTA |= txmask;
  _DIRA |= txmask;

  value = (c | 256) << 1;
  waitcycles = getcnt() + bitcycles;
  for (i = 0; i < 10; i++)
    {
      waitcycles = __builtin_propeller_waitcnt(waitcycles, bitcycles);
#if 0
      if (value & 1)
        _OUTA |= txmask;
      else
        _OUTA &= ~txmask;
      value >>= 1;
#else
      value = shiftout(value, txmask, _OUTA);
#endif
    }
  // if we turn off DIRA, then some boards (like QuickStart) are left with
  // floating pins and garbage output; if we leave it on, we're left with
  // a high pin and other cogs cannot produce output on it
  // the solution is to use FullDuplexSerialDriver instead on applications
  // with multiple cogs
  //_DIRA &= ~txmask;
  return c;
}

int
_serial_putchar(int c)
{
    static unsigned int txmask;
    static unsigned int bitcycles;
    static unsigned int cached_baud;

    if (cached_baud != _baud) {
        cached_baud = _baud;
        bitcycles = _clkfreq / cached_baud;
        txmask = (1UL << _txpin);
    }
    if (c == '\n') {
        _serial_tx('\r', txmask, bitcycles);
    }
    return _serial_tx(c, txmask, bitcycles);
}

/*
+--------------------------------------------------------------------
¦  TERMS OF USE: MIT License
+--------------------------------------------------------------------
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
+------------------------------------------------------------------
*/
