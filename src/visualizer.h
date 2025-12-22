#ifndef _VISUALIZATION_H
#define _VISUALIZATION_H

#include <stddef.h>

void visualization_init(int sampleRate, const float *audio_buffer, size_t buffer_size);
void visualization_run(float *(*fft_callback)(void));
void visualization_terminate(void);

#endif