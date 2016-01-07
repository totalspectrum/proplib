#include <stdio.h>
#include <stdint.h>
#include <time.h>
#ifdef __CATALINA__
#include <propeller.h>
#else
#include <sys/time.h>
#endif
#define INF __builtin_inff()
#define NAN __builtin_nanf("0")

//#define VERBOSE

#define FTYPE float
#define UFINT uint32_t
#define FINT  int32_t
#define FL(x) x ## f

union fi { FTYPE f; UFINT i; };

FTYPE asFloat(UFINT i)
{
    union fi x; x.i = i; return x.f;
}
UFINT asInt(FTYPE f)
{
    union fi x; x.f = f; return x.i;
}

int is_a_nan(UFINT x)
{
    int exp = (x >> 23) & 0xff;
    int mant = (x & 0x7fffff);
    return (exp == 0xff) && mant != 0;
}

int differ(UFINT x, UFINT y)
{

    if (x == y)
        return 0;
    /* if both are NaN, accept them both */
    if (is_a_nan(x) && is_a_nan(y))
        return 0;
    return 1;
}

int
validatefunc(FTYPE a, FTYPE b, UFINT c, FTYPE expect, int op)
{
    union fi { FTYPE f; UFINT i; } ua, ub, ue;
    int flaw = 0;

    ua.f = a;
    ub.f = b;
    ue.f = expect;
    if (differ(c, ue.i)) {
        printf("%8lx %c %8lx -> %8lx (should be %8lx)\n",
               (long)ua.i, op, (long)ub.i, (long)c, (long)ue.i);
        flaw++;
    }
#ifdef VERBOSE
    else {
        printf("%8lx %c %8lx -> %8lx ok\n",
               (long)ua.i, op, (long)ub.i, (long)c);
    }
#endif
    return flaw;
}

FTYPE testadd(FTYPE a, FTYPE b)
{
    return a + b;
}

int
validateaddfunc(FTYPE a, FTYPE b, FTYPE expect)
{
    int flaw;
    UFINT c;
    c = asInt(testadd(a, b));
    flaw = validatefunc(a, b, c, expect, '+');
    c = asInt(testadd(b, a));
    flaw += validatefunc(b, a, c, expect, '+');
    return flaw;
}

int
validatemulfunc(FTYPE a, FTYPE b, FTYPE expect)
{
    int flaw;
    UFINT c;
    c = asInt(a*b);
    flaw = validatefunc(a, b, c, expect, '*');
    c = asInt(b*a);
    flaw += validatefunc(b, a, c, expect, '*');
    return flaw;
}

int
validatedivfunc(FTYPE a, FTYPE b, FTYPE expect)
{
    int flaw;
    UFINT c;
    c = asInt(a/b);
    flaw = validatefunc(a, b, c, expect, '/');
    return flaw;
}

int
validateconvfunc(FTYPE a, int64_t expect)
{
    union fi { FTYPE f; UFINT i; } ua;
    int64_t b = (int64_t)a;
    if (b != expect)
    {
        ua.f = a;
        printf("%8lx -> %lld (should be %lld)\n", (long)ua.i, (long long)b, (long long)expect);
        return 1;
    }
    return 0;
}

double
dtime()
{
#ifdef __CATALINA__
    return (double)((unsigned long)CNT)/80000000.0;
#else
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + (tv.tv_usec / 1000000.0);
#endif
}

void
runaddtiming()
{
    double elapsed;
    FTYPE r;
    int i;

    r = 0.0;
    elapsed = dtime();
    for (i = 0x3f7eff00; i < 0x3f800123; i++) {
        r += asFloat(i);
    }
    elapsed = dtime() - elapsed;
    printf(" result %f (0x%08x), elapsed time %f\n", r, asInt(r), elapsed);
}
void
runmultiming()
{
    double elapsed;
    FTYPE r;
    int i;

    r = 1.0;
    elapsed = dtime();
    for (i = 0x3f7ffff0; i < 0x3f801223; i++) {
        r *= asFloat(i);
    }
    elapsed = dtime() - elapsed;
    printf(" result %f (0x%08x), elapsed time %f\n", r, asInt(r), elapsed);
}
void
rundivtiming()
{
    double elapsed;
    FTYPE r;
    int i;

    r = 1.0;
    elapsed = dtime();
    for (i = 0x3f7ffff0; i < 0x3f801223; i++) {
        r /= asFloat(i);
    }
    elapsed = dtime() - elapsed;
    printf(" result %f (0x%08x), elapsed time %f\n", r, asInt(r), elapsed);
}

#define validateadd(a, b) errs += validateaddfunc((a), (b), ((a)+(b)))
#define validatemul(a, b) errs += validatemulfunc((a), (b), ((a)*(b)))
#define validatediv(a, b, c) errs += validatedivfunc((a), (b), (c))
#define validateconv(a, b) errs += validateconvfunc((a), (b))

void
validateunpackfunc(uint32_t A)
{
#if 0
    extern uint32_t testman(uint32_t);
    extern uint32_t testexp(uint32_t);
    extern uint32_t testflag(uint32_t);
    extern uint32_t testpack(uint32_t, uint32_t, uint32_t);
    uint32_t m, e, f, Z;

    m = testman(A);
    e = testexp(A);
    f = testflag(A);
    Z = testpack(m, e, f);
    printf("A=0x%08x m=0x%08x e=0x%08x f=0x%x Z=0x%08x\n", A, m, e, f, Z);
#endif
}

int
main()
{
    int errs = 0;

#if 0
    validateunpackfunc(0x3f800000);
    validateunpackfunc(0x3fb33333);
    validateunpackfunc(0x40000000);
    validateunpackfunc(0x447a0000);
#endif

    validatedivfunc(1.0f, 1.0f, 1.0f);
    printf("running float tests...\n");

    printf("conversion:\n");
    validateconv(0.0f, 0);
    validateconv(1.0f, 1);
    validateconv(-100.0f, -100);
    validateconv(       128000.0f,        128000); // 128K 
    validateconv(    128000000.0f,     128000000); // 128M
    validateconv(   1280000000.0f,    1280000000); // 1.28G
    validateconv(-128000000000.0f, -128000000000); // 128G

    printf("basic addition:\n");
    validateadd(0.0, 1.0);
    validateadd(1.0, 1.0);
    validateadd(-2.0, 1.0);
    validateadd(1.0, 2.0);
    validateadd(1.4, 3.7);
    validateadd(1.1, 0.9);
    validateadd(1000.0,  1.0);
    validateadd(FL(3.1), FL(0.0001));

    /* subtraction */
    printf("basic subtraction:\n");
    validateadd(1.0, -1.0);
    validateadd(-1.0, 0.5);
    validateadd(1.0, -100.0);
    validateadd(100.0, -1.0);
    validateadd(1.0, -0.5);
    validateadd(8.0, -4.0);

    errs += validateaddfunc(asFloat(0x3f7fffff), asFloat(0xbf800000), asFloat(0xb3800000));
    errs += validateaddfunc(asFloat(0x3f800000), asFloat(0xbf800000), asFloat(0));
    errs += validateaddfunc(asFloat(0x3f7fffff), asFloat(0xbf7fffff), asFloat(0));
#ifndef __CATALINA__
    /* check for round to nearest even */
    validateadd(0x1.FFFFFEp-1, -0x0.000001p-1);
    validateadd(0x1.FFFFFCp-1, -0x0.000001p-1);

    printf("denormalized numbers:\n");
    validateadd(FL(0x1.0p-129), FL(0x1.0p-132));
    validateadd(FL(0x1.0p-127), FL(0x1.0p-127));
    validateadd(FL(0x1.0p-127), FL(0x1.0p-128));
    validateadd(FL(0x1.0p-127), FL(-0x1.0p-128));

    printf("special cases:\n");
    validateadd(0.0, 0.0);
    validateadd(1.0, INF);
    validateadd(-0.0, 0.0);
    validateadd(-0.0, -0.0);
    validateadd(INF, -INF);
    validateadd(NAN, 2.0);
    validateadd(NAN, INF);
    errs += validateaddfunc(FL(0x1.9p127), FL(0x0.AAAAp127), INF);
#endif

    printf("multiply:\n");
    validatemul(1.0,  1.0);
    validatemul(2.0,  2.0);
    validatemul(2.0,  -2.0);
    validatemul(100.0, -0.1);
    validatemul(-0.0, 0.0);
    validatemul(-0.0, 2.0);
    validatemul(2.0, INF);
    validatemul(-0.0, INF);
    validatemul(-INF, INF);

    printf("division:\n");
    validatediv(1.0f, 1.0f, 1.0f);
    validatediv(-1.0f, 2.0f, -0.5f);
    validatediv(100.0f, 10.0f, 10.0f);
    validatediv(0.0f,0.0f,NAN);
    validatediv(1.0f,0.0f,INF);
    validatediv(INF, 2.0f,INF);
    validatediv(1.0f,-INF,-0.0f);
    validatediv(NAN,-INF,NAN);
    validatediv(NAN,-1.0f,NAN);

    if (errs == 0) {
        printf(" all ok!!\n");
        printf("add timing: "); fflush(stdout);
        runaddtiming();
        printf("mul timing: "); fflush(stdout);
        runmultiming();
        printf("div timing: "); fflush(stdout);
        rundivtiming();
        printf("done\n");
    }
    return errs;
}
