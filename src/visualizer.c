#include "visualizer.h"
#include "fft.h"
#include <math.h>
#include <stdio.h>
#include <SDL3/SDL.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define BARS 128
#define BINS_PER_BAR (FFT_SIZE / 2 / BARS)
#define BAR_WIDTH ((float)WINDOW_WIDTH / (float)BARS)
#define BAR_SPEED 30.0f

static float currentBars[BARS];
static float desiredBars[BARS];
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_AudioStream *stream;
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
    audioBuffer = audio_buffer;
    bufferSize = buffer_size;

    printf("Visualizer initialized with sample rate: %d Hz\n", sampleRate);
}

static void _update_desired_bars(float *fft_buffer)
{
    for (int i = 0; i < BARS; i++)
    {
        float sum = 0.0f;

        for (int j = 0; j < BINS_PER_BAR; j++)
        {
            sum += fft_buffer[i * BINS_PER_BAR + j];
        }

        desiredBars[i] = 10 * log10f(sum / BINS_PER_BAR + 1e-6f);
        desiredBars[i] = (desiredBars[i] + 60.0f) * 10.0f;

        if (desiredBars[i] < 0.0f)
        {
            desiredBars[i] = 0.0f;
        }
    }
}

static void _update_current_bars(float dt)
{
    for (int i = 0; i < BARS; i++)
    {
        if (currentBars[i] < desiredBars[i])
        {
            currentBars[i] += BAR_SPEED * dt * (desiredBars[i] - currentBars[i]);
        }
        else
        {
            currentBars[i] -= BAR_SPEED * dt * (currentBars[i] - desiredBars[i]);
        }
    }
}

static void _render_bars()
{
    SDL_SetRenderDrawColor(renderer, 10, 10, 15, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 200, 100, 255);
    for (int i = 0; i < BARS; i++)
    {
        SDL_FRect bar = {
            .x = i * BAR_WIDTH + 2,
            .y = WINDOW_HEIGHT - currentBars[i],
            .w = BAR_WIDTH - 4,
            .h = currentBars[i]};
        SDL_RenderFillRect(renderer, &bar);
    }
    SDL_RenderPresent(renderer);
}

void visualization_run(float *(*fft_callback)(void))
{
    SDL_PutAudioStreamData(stream, audioBuffer, bufferSize * sizeof(float));

    bool quit = false;
    SDL_Event event;
    Uint64 lastFrameTimeNS = SDL_GetTicksNS();
    int lastNumberOfFFTs = -1;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }

        int samplesPushed = bufferSize - SDL_GetAudioStreamQueued(stream) / sizeof(float);
        int currentNumberOfFFTs = samplesPushed / (FFT_SIZE / 2);

        if (currentNumberOfFFTs != lastNumberOfFFTs)
        {
            lastNumberOfFFTs = currentNumberOfFFTs;

            float *buffer = fft_callback();

            _update_desired_bars(buffer);
        }

        float dt = (SDL_GetTicksNS() - lastFrameTimeNS) / 1e9f;
        lastFrameTimeNS = SDL_GetTicksNS();

        _update_current_bars(dt);
        _render_bars();
    }
}

void visualization_terminate(void)
{
    SDL_DestroyAudioStream(stream);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}