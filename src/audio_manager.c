#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "audio_constants.h"

static Mix_Music* background_audio_mixer_buffer;
static Mix_Chunk* audio_mixer_buffer;

char* constant_to_audio_buffer(int audio_constant) {
  switch (audio_constant) {
    case 1: {
      return "./assets/sounds/rae_lgc.mp3";
    }
    case 2: {
      return "./assets/sounds/aot.mp3";
    }
  }
}

bool initialize_mixer_context(void) {
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
    fprintf(stderr, "Error initializing Mixer. %s\n", Mix_GetError());
    return false;
  }
  return true;
}

bool load_background_audio(const char* audio_buffer) {
  background_audio_mixer_buffer = Mix_LoadMUS(audio_buffer);
  if (!background_audio_mixer_buffer) {
    fprintf(stderr, "Error initializing Music. %s\n", Mix_GetError());
    return false;
  }
  return true;
}

void play_background_audio(int loop) {
  Mix_PlayMusic(background_audio_mixer_buffer, loop);
}

bool load_audio(int audio_buffer) {
 // audio_buffer = Mix_LoadWAV("./assets/");
  if (!audio_mixer_buffer) {
    fprintf(stderr, "Error initializing Audio. %s\n", Mix_GetError());
    return false;
  }
  return true;
}

void play_audio(int loop) {
  //
}

void destroy_mixer_context(void) {
}
