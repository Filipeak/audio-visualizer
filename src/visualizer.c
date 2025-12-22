#include "visualizer.h"
#include "fft.h"
#include <math.h>
#include <stdio.h>
#include <SDL3/SDL.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define BARS 64
#define BINS_PER_BAR (FFT_SIZE / 2 / BARS)
#define BAR_WIDTH ((float)WINDOW_WIDTH / (float)BARS)

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_AudioStream *stream;
static float dt;
static const float *audioBuffer;
static size_t bufferSize;

void visualization_init(int sampleRate, const float *audio_buffer, size_t buffer_size)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_AudioSpec spec = {SDL_AUDIO_F32, 1, sampleRate};
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(stream);

    window = SDL_CreateWindow(
        "FFT Visualizer",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    renderer = SDL_CreateRenderer(window, NULL);
    dt = (FFT_SIZE / 2) / (float)sampleRate * 1000;
    audioBuffer = audio_buffer;
    bufferSize = buffer_size;

    printf("Visualizer initialized with sample rate: %d Hz\n", sampleRate);
}

static void _render_visualization(float *fft_buffer)
{
    float bars[BARS];

    for (int i = 0; i < BARS; i++)
    {
        float sum = 0.0f;

        for (int j = 0; j < BINS_PER_BAR; j++)
        {
            sum += fft_buffer[i * BINS_PER_BAR + j];
        }

        bars[i] = 10 * log10f(sum / BINS_PER_BAR + 1e-6f);
        bars[i] = (bars[i] + 60.0f) * 10.0f;

        if (bars[i] < 0.0f)
        {
            bars[i] = 0.0f;
        }
    }

    SDL_SetRenderDrawColor(renderer, 10, 10, 15, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 200, 100, 255);
    for (int i = 0; i < BARS; i++)
    {
        SDL_FRect bar = {
            .x = i * BAR_WIDTH + 2,
            .y = WINDOW_HEIGHT - bars[i],
            .w = BAR_WIDTH - 4,
            .h = bars[i]};
        SDL_RenderFillRect(renderer, &bar);
    }
    SDL_RenderPresent(renderer);
}

void visualization_run(float *(*fft_callback)(void))
{
    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        Uint64 frame_start = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }

        SDL_PutAudioStreamData(stream, audioBuffer, bufferSize * sizeof(float));

        float *buffer = fft_callback();

        _render_visualization(buffer);

        Uint64 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < dt)
        {
            SDL_Delay(dt - frame_time);
        }
    }
}

void visualization_terminate(void)
{
    SDL_DestroyAudioStream(stream);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}