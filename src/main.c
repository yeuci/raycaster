#include "defs.h"
#include "textures.h"
#include "graphics.h"
#include "audio_constants.h"
#include "audio_manager.h"
#include "map.h"
#include "ray.h"
#include "player.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "wall.h"

bool game_running                 = false;
int last_frame_ticks              = 0;

void process_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT: {
        game_running = false;
        break;
      }

      case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          game_running = false;
        }
        if (event.key.keysym.sym == SDLK_r) {
          load_background_audio(AC_AOT_RAGE);
          play_background_audio(-1);
        }
        if (event.key.keysym.sym == SDLK_t) {
          load_background_audio(AC_RAE_LGC);
          play_background_audio(-1);
        }
        if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
          player.walkDirection = 1;
        }
        if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
          player.walkDirection = -1;
        }
        if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
          player.turnDirection = 1;
        }
        if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
          player.turnDirection = -1;
        }
        break;
      }

      case SDL_KEYUP: {
        if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
          player.walkDirection = 0;
        }
        if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
          player.walkDirection = 0;
        }
        if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
          player.turnDirection = 0;
        }
        if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
          player.turnDirection = 0;
        }
        break;
      }
    }
  }
}

void setup() {
  load_world_textures();
}

void update() {
  int time_to_wait = FRAME_TIME_LENGTH - (SDL_GetTicks() - last_frame_ticks);

  if (time_to_wait > 0 && time_to_wait <= FRAME_TIME_LENGTH) {
    SDL_Delay(time_to_wait);
  }

  float delta_time = (SDL_GetTicks() - last_frame_ticks) / 1000.0f;

  last_frame_ticks = SDL_GetTicks();

  move_player(delta_time);
  cast_all_rays();
}

void render() {
  clear_color_buffer(0xFF000000);

  generate_projection();
  //generate_object_projection();

  render_map();
  render_rays();
  render_player();

  render_color_buffer();
}

void free_resources(void) {
  free_world_textures();
  destroy_window();
}

int main(int argc, char** argv) {
  game_running = initialize_window() && initialize_mixer_context();

  if (!game_running) {
    printf("There was an error on initialization.\n");
    return EXIT_FAILURE;
  }

  if (!load_background_audio(AC_AOT_RAGE)) {
    printf("There was an error loading audio buffer.\n");
    return EXIT_FAILURE;
  }

  //play_background_audio(-1);

  setup();

  while (game_running) {
    process_input();
    update();
    render();
  }

  free_resources();

  return EXIT_SUCCESS;
}
