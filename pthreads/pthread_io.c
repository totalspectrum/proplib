/*
 * default _InitIO function to set up stdio, stderr, etc.
 * This differs from the one in the "normal" library because
 * SimpleSerialDriver is not well suited to multiply threaded
 * programs (it may not work right if called on different cogs)
 * FullDuplexSerialDriver does not have this issue.
 */

#include <stdio.h>
#include <stdarg.h>
#include <driver.h>
#include <compiler.h>
#include <sys/thread.h>
/* list of drivers we can use */
extern _Driver _FullDuplexSerialDriver;

_Driver *_driverlist[] = {
  &_FullDuplexSerialDriver,
  NULL
};

int printf(const char *fmt, ...)
{
    va_list args;
    int r;
    __lock(&stdout->_lock);
    va_start(args, fmt);
    r = _dofmt( (_FmtPutfunc)fputc, stdout, fmt, &args);
    va_end(args);
    __unlock(&stdout->_lock);
    return r;
}

int
puts(const char *str)
{
    int c;
    int r = 0;
    __lock(&stdout->_lock);
    while ( (c = *str++) != 0 )
        r |= fputc(c, stdout);
    r |= fputc('\n', stdout);
    __unlock(&stdout->_lock);
    return r;
}
