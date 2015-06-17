//
// Simple implementation of itoa
// Written by Eric R. Smith and placed in the public domain
//
#include <stdlib.h>

char *
itoa( int value, char *origbuf, int base )
{
    char *buf = origbuf;
    if (value < 0 && base == 10) {
        *buf++ = '-';
        value = -value;
    }
    _itoa_prec(value, buf, base, 1);
    return origbuf;
}

#ifdef TEST
#include <stdio.h>
int main ()
{
  int i;
  char buffer [33];
  printf ("Enter a number: ");
  scanf ("%d",&i);
  itoa (i,buffer,10);
  printf ("decimal: %s\n",buffer);
  itoa (i,buffer,16);
  printf ("hexadecimal: %s\n",buffer);
  itoa (i,buffer,2);
  printf ("binary: %s\n",buffer);
  return 0;
}

#endif	

