#ifndef _MP3_H
#define _MP3_H

#include <stddef.h>

void process_mp3(const char *input_path, float **mono_signal, size_t *mono_samples, int* hz);

#endif