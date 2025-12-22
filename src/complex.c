#include "complex.h"
#include <math.h>

complex_t complex_add(complex_t *a, complex_t *b)
{
    complex_t result;
    result.re = a->re + b->re;
    result.im = a->im + b->im;
    return result;
}

complex_t complex_sub(complex_t *a, complex_t *b)
{
    complex_t result;
    result.re = a->re - b->re;
    result.im = a->im - b->im;
    return result;
}

complex_t complex_mul(complex_t *a, complex_t *b)
{
    complex_t result;
    result.re = a->re * b->re - a->im * b->im;
    result.im = a->re * b->im + a->im * b->re;
    return result;
}

float complex_mag(complex_t *a)
{
    return sqrtf(a->re * a->re + a->im * a->im);
}