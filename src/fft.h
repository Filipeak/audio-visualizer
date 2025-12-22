#ifndef _FFT_H
#define _FFT_H

#include <stddef.h>

#define FFT_SIZE 2048

void apply_hann_window(float *buffer);
void processFFT(float *window_buffer);

#endif