/**
 * @file include/alloca.h
 *
 * @brief Provides the alloca function to allocate memory on the stack.
 *
 */

#ifndef _LIB_ALLOCA_H
#define _LIB_ALLOCA_H

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * The alloca function allocates size bytes of space on the stack
 * frame of its caller. This space is automatically freed when
 * that caller returns. If the allocation causes stack overflow,
 * program behavior is undefined (and likely to be very unpleasant).
 * The alloca function is not part of the C standard, but is a useful
 * utility that was introduced in the BSD version of Unix.
 */
#ifdef __GNUC__
#define alloca __builtin_alloca
#else
    void *alloca(size_t size);
#endif

#if defined(__cplusplus)
}
#endif

#endif
