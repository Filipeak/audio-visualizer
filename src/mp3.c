#include "mp3.h"
#include <stdio.h>
#include <stdlib.h>

#define MINIMP3_ONLY_MP3
#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_IMPLEMENTATION
#include "minimp3_ex.h"

static void _convert_to_mono(const float *stereo_buffer, size_t stereo_samples, float **mono_buffer, size_t *mono_samples, int channels)
{
    *mono_samples = stereo_samples / channels;
    *mono_buffer = (float *)malloc(*mono_samples * sizeof(float));

    for (size_t i = 0, j = 0; i < stereo_samples; i += channels, j++)
    {
        float sum = 0.0f;

        for (int ch = 0; ch < channels; ch++)
        {
            sum += stereo_buffer[i + ch];
        }

        (*mono_buffer)[j] = sum / channels;
    }

    printf("Converted to mono signal with %zu samples.\n", *mono_samples);
}

void process_mp3(const char *input_path, float **mono_signal, size_t *mono_samples, int* hz)
{
    mp3dec_t mp3d;
    mp3dec_file_info_t info;

    printf("Processing mp3 file: %s\n", input_path);

    if (mp3dec_load(&mp3d, input_path, &info, NULL, NULL))
    {
        printf("Error loading mp3 file: %s\n", input_path);

        return;
    }

    printf("Decoded %zu samples, %d channels, %d Hz, layer %d, avg bitrate %d kbps\n", info.samples, info.channels, info.hz, info.layer, info.avg_bitrate_kbps);

    *hz = info.hz;

    _convert_to_mono(info.buffer, info.samples, mono_signal, mono_samples, info.channels);

    free(info.buffer);
}