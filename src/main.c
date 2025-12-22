#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3.h"
#include "fft.h"
#include "visualizer.h"

static float *mono_signal = NULL;
static size_t mono_samples = 0;
static float windowed_buffer[FFT_SIZE];
static size_t current_position = 0;

static float* run_next_fft()
{
    if (current_position + FFT_SIZE < mono_samples)
    {
        memcpy(windowed_buffer, mono_signal + current_position, FFT_SIZE * sizeof(float));

        apply_hann_window(windowed_buffer);
        processFFT(windowed_buffer);

        current_position += FFT_SIZE / 2;
    }

    return windowed_buffer;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Invalid parameters. Usage: %s <input_file>\n", argv[0]);

        return 1;
    }

    char *input_path = argv[1];
    printf("Input path: %s\n", input_path);

    int hz = 0;
    process_mp3(input_path, &mono_signal, &mono_samples, &hz);

    visualization_init(hz, mono_signal, mono_samples);
    visualization_run(run_next_fft);
    visualization_terminate();

    free(mono_signal);

    printf("Program terminated successfully.\n");

    return 0;
}