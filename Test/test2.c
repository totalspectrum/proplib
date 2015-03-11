#include <stdio.h>

extern double _intpow(double a, double b, int n);

float tof(double d)
{
    return (float)d;
}

int asint(float f)
{
    union { float f; int i; } u;
    u.f = f;
    return u.i;
}
long long aslong(double f)
{
    union { double f; long long i; } u;
    u.f = f;
    return u.i;
}

void
dotest(const char *str, double d)
{
    float c = tof(d);
    printf("%s: d = %llx float = 0x%x\n", str, aslong(d), asint(c));
}

double theval = 0.1;
float thefloatval = 0.1f;

int
main()
{
    double d;

    d = _intpow(1.0, 10.0, -1);
    dotest("from intpow -1", d);
    d = theval;
    dotest("from var", d);
    d = _intpow(1.0, 10.0, 1);
    dotest("from intpow +1", d);
    dotest("from var again", theval);
    dotest("saved intpow", d);
    dotest("const", 1.0);
    return 0;
}
