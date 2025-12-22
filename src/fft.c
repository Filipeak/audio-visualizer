#include "fft.h"
#include "complex.h"

#define _USE_MATH_DEFINES
#include <math.h>

static float _hann_window(size_t n, size_t N)
{
    return 0.5f * (1.0f - cosf((2.0f * M_PI * n) / (N - 1)));
}

void apply_hann_window(float *buffer)
{
    for (size_t i = 0; i < FFT_SIZE; i++)
    {
        buffer[i] *= _hann_window(i, FFT_SIZE);
    }
}

void _fft_recursive(complex_t *x, int n)
{
    if (n <= 1)
    {
        return;
    }

    complex_t even[n / 2];
    complex_t odd[n / 2];

    for (int i = 0; i < n / 2; i++)
    {
        even[i] = x[2 * i];
        odd[i] = x[2 * i + 1];
    }

    _fft_recursive(even, n / 2);
    _fft_recursive(odd, n / 2);

    for (int k = 0; k < n / 2; k++)
    {
        float angle = -2.0f * M_PI * k / n;

        complex_t w = {cosf(angle), sinf(angle)};
        complex_t t = complex_mul(&w, &odd[k]);

        x[k] = complex_add(&even[k], &t);
        x[k + n / 2] = complex_sub(&even[k], &t);
    }
}

void processFFT(float *window_buffer)
{
    complex_t x[FFT_SIZE];

    for (size_t i = 0; i < FFT_SIZE; i++)
    {
        x[i].re = window_buffer[i];
        x[i].im = 0.0f;
    }

    _fft_recursive(x, FFT_SIZE);

    for (int i = 0; i < FFT_SIZE / 2; i++)
    {
        float magnitude = complex_mag(&x[i]);

        window_buffer[i] = magnitude;
    }
}