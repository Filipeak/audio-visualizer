#ifndef _COMPLEX_H
#define _COMPLEX_H

typedef struct complex
{
    float re;
    float im;
} complex_t;

complex_t complex_add(complex_t *a, complex_t *b);
complex_t complex_sub(complex_t *a, complex_t *b);
complex_t complex_mul(complex_t *a, complex_t *b);
float complex_mag(complex_t *a);

#endif