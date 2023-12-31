#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

// Play a sine wav using SDL's callback function

#define SECONDS 6
#define CHANNELS 2
#define PI 3.141592

typedef struct
{
    int size;
    int play_cursor;
    int bytes_per_period;
    Sint16 *buffer;
} Audio_Data;

void audio_callback(void *userdata, Uint8 *stream, int len);

int main(int argc, char *argv[])
{
    printf("Playing a sine wave.\n");

    // Using 44kHz here over 44.1kHz so the bytes per cycle line up evenly.
    // @Todo: update this to handle any bytes per cycle value
    int samples_per_second = 44000;
    int bytes_per_sample = sizeof(Uint16) * CHANNELS;

    // If we multiply bytes_to_write by SECONDS we would create a wave that
    // fills all SECONDS of playing time. However, we can write a smaller wave,
    // once SDL asks for a location past the end of the wave just pass it the
    // beginning of the wave at the same point in the period
    int bytes_to_write = samples_per_second * bytes_per_sample;

    void *sound_buffer = malloc(bytes_to_write);
    Sint16 *sample_write = (Sint16 *)sound_buffer;
    Sint16 *sample_start = sample_write;

    int tone_hz = 440;
    Uint16 tone_volume = 5000;
    int sine_wave_period = samples_per_second / tone_hz; // Samples in a period (we use 44kHz so this would line up evenly)
    int sample_count = bytes_to_write / bytes_per_sample;
    Sint16 sample_value;

    for (int sample_index = 0; sample_index < sample_count; sample_index++)
    {

        // Create the sine wav
        double two_pi = 2.0 * PI;
        double t = (double)sample_index / (double)samples_per_second;
        double sval = tone_volume * sin(two_pi * (double)tone_hz * t);
        sample_value = (Sint16)sval;

        // Write the sample_value to the buffer for each channel
        for (int channel = 0; channel < CHANNELS; channel++)
        {
            *sample_write++ = sample_value;
        }
    }

    // Init SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("SDL init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_AudioSpec wanted_spec;
    SDL_AudioSpec obtained_spec;
    Audio_Data audio_data;

    audio_data.buffer = sample_start;
    audio_data.play_cursor = 0;
    audio_data.size = bytes_to_write;
    audio_data.bytes_per_period = sine_wave_period * bytes_per_sample;

    wanted_spec.freq = samples_per_second;
    wanted_spec.format = AUDIO_S16;
    wanted_spec.channels = CHANNELS;
    wanted_spec.samples = 4096; // SDL buffer sample size (same used for wav files)
    wanted_spec.callback = audio_callback;
    wanted_spec.userdata = &audio_data;

    // Open the audio device
    int iscapture = 0;       // We are not opening the device for recording so iscapture = 0
    int allowed_changes = 0; // See docs for info on what this is for

    SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, iscapture, &wanted_spec, &obtained_spec, allowed_changes);
    if (device == 0)
    {
        printf("SDL_OpenAudioDevice error: %s\n", SDL_GetError());
        return 1;
    }

    // Start playing
    SDL_PauseAudioDevice(device, 0);

    // Wait for SECONDS number of seconds
    SDL_Delay(SECONDS * 1000);

    // Shut everything down
    SDL_CloseAudioDevice(device);
    free(sound_buffer);
    SDL_Quit();
}

// This is our custom Audio callback function, SDL will call this function when
// it is ready for more data (audio) to be output from the audio device
void audio_callback(void *userdata, Uint8 *stream, int len)
{

    // Cast the userdata to Audio_Data so we can use it
    Audio_Data *audio_data = (Audio_Data *)userdata;

    // If SDL asks for data past the end of the generated wave we need to just
    // give it the point in the period that it asked for but at the beginning
    // of the generated wave. We can do this by taking the modulus of the
    // play_cursor and the bytes_per_period
    if (audio_data->play_cursor + len > audio_data->size)
    {
        audio_data->play_cursor %= audio_data->bytes_per_period;
    }

    memcpy(stream, ((Uint8 *)audio_data->buffer + audio_data->play_cursor), len);
    audio_data->play_cursor += len;
}