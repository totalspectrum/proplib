//
// make a string lower case
// Written by Eric R. Smith (ersmith@totalspectrum.ca)
// and placed in the public domain
//

#include <stdio.h>
#include <ctype.h>
#include <string.h>

//
// lower case a string in-place
//
char* _strlwr(char *origstr)
{
    char *str = origstr;
    int c;
    while ( (c = *str) != 0 ) {
        if (isupper(c)) {
            *str = tolower(c);
        }
        str++;
    }
    return origstr;
}
