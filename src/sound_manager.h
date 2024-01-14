#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <stdbool.h>

bool initialize_mixer_context(void);
void destroy_mixer_context(void);

bool load_background_audio(int loop);
void play_background_audio(int audio_constant);

char* constant_to_audio_buffer(int audio_buffer);

bool load_audio();
void play_audio(int audio_buffer);

#endif
