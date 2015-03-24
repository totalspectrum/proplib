/**
 * @file include/sys/serial.h
 * @brief Provides definitions for some low level serial port routines
 *
 * Copyright (c) 2015 by Total Spectrum Software Inc.
 * MIT Licensed
 */
#ifndef _SYS_SERIAL_H
#define _SYS_SERIAL_H

#include <compiler.h>

/**
 * @brief Transmit a byte out a pin
 *
 * @details This function transmits a single byte. The transmission is
 * half-duplex (i.e. no bytes can be received while it is happening) and
 * uses the current cog.
 *
 * @param c The 8-bit character to transmit (upper 24 bits must be 0).
 * @param txmask A bit mask with a 1 set to indicate the pin to use, 0 elsewhere.
 * @param bitcycles Time (in CNT cycles) to wait between transmitting bits.
 */
int _serial_tx(int c, unsigned int txmask, unsigned int bitcycles);

/**
 * @brief Transmit a byte out the serial port.
 *
 * @details This function transmits a single byte using _serial_tx.
 * The pin and baud rate are selected from the defaults that can be
 * changed by the loader.
 *
 * @param c The 8-bit character to transmit (upper 24 bits are ignored).
 */
int _serial_putchar(int c);

/**
 * @brief Define a default putchar which will use the serial port.
 *
 * @details This macro defines a default for putchar which will
 * use _serial_putchar. The definition is "weak", so multiple uses
 * of it are harmless, and will be overridden by any "strong" (normal)
 * definition for putchar()
 */
#define _DEFAULT_SERIAL_PUTCHAR \
    _WEAK int putchar(int c) { return _serial_putchar(c); }


#endif
