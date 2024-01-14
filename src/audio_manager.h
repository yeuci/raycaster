#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <stdbool.h>

bool initialize_mixer_context(void);
void destroy_mixer_context(void);

bool load_background_audio(const char* audio_buffer);
void play_background_audio(int loop);

char* constant_to_audio_buffer(int audio_constant);

bool load_audio(const char* audio_buffer);
void play_audio(int loop);

#endif
