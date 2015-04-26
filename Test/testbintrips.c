//
// test round trip double->dec->double
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#if 0
extern int mysnprintf( char *buf, size_t len, const char *fmt, ...);
extern double myatof(const char *);
#define MYATOF(x) myatof(x)
#define MYSNPRINTF mysnprintf
#else
#define MYATOF(x) strtod(x, NULL)
#define MYSNPRINTF snprintf
#endif

typedef union DI {
  double d;
  uint64_t i;
  uint8_t b[8];
} DI;

double rand_double(void)
{
  DI u;
  int i;

  for (i = 0; i < 8; i++) {
    u.b[i] = rand();
  }
  return u.d;
}

static int fail = 0;

void mismatch(double orig, double r, char *buf)
{
  fprintf(stderr, "mismatch: orig=%.13a, got=%.13a buf=[%s]\n", orig, r, buf);
  fail++;
}

void test(double d)
{
  char buf[128];
  double x;

  MYSNPRINTF(buf, sizeof(buf), "%.17e", d);
  x = MYATOF(buf);
  if (isnan(d)) {
    if (!isnan(x)) {
      mismatch(d, x, buf);
    }
  } else if (x != d) {
    mismatch(d, x, buf);
  }
}

#define TRIES 50000
#define TRYTICK 1000

int
main(void)
{
  int i;
  double x;

  srand(123);
  for (i = 0; i < TRIES; i++) {
      if ( (i % TRYTICK) == 0 ) {
          printf("%d...", i);
          fflush(stdout);
      }
      x = rand_double();
      test(x);
  }
  printf("%d failures out of %d tests\n", fail, TRIES);
  return 0;
}
