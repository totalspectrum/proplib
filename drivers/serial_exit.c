/*
 * simple serial driver
 * Copyright (c) 2015 Total Spectrum Software Inc.
 * MIT Licensed (see end of file)
 */

/*
 * special hook which sends a sequence which propeller-load recognizes
 * as an exit
 */

#include <sys/serial.h>

void
_serial_exit(int n)
{
  _serial_putchar(0xff);
  _serial_putchar(0x00);
  _serial_putchar(n & 0xff);
  __builtin_propeller_cogstop(__builtin_propeller_cogid());
}

#ifdef __GNUC__
void _ExitHook(int) __attribute__ ((alias ("_serial_exit")));
#else
void _ExitHook(int n)
{
  _serial_exit(n);
}
#endif
