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
#include <limits.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

bool game_running                 = false;
int last_frame_ticks              = 0;

void render_rays() {
  /*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (int i = 0; i < NUM_RAYS; i++) {
    SDL_RenderDrawLine(
      renderer,
      MINIMAP_SCALE_FACTOR * player.x,
      MINIMAP_SCALE_FACTOR * player.y,
      MINIMAP_SCALE_FACTOR * rays[i].wallHitX,
      MINIMAP_SCALE_FACTOR * rays[i].wallHitY
    );
  }
  */
}

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

void generate_projection() {
  for (int x = 0; x < NUM_RAYS; x++) {
    float p_distance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);
    float projected_wall_height = (TILE_SIZE / p_distance) * DIST_PROJ_PLANE;

    int wall_strip_height = (int)projected_wall_height;

    int wall_top_pixel = (WINDOW_HEIGHT / 2) - (wall_strip_height / 2);
    wall_top_pixel = wall_top_pixel < 0 ? 0 : wall_top_pixel;

    int wall_bottom_pixel = (WINDOW_HEIGHT / 2) + (wall_strip_height / 2);
    wall_bottom_pixel = wall_bottom_pixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wall_bottom_pixel;

    for (int y = 0; y < wall_top_pixel; y++) {
      draw_pixel_to_buffer(x, y, 0xFF333333);
    }

    int to_x;
    if (rays[x].wasHitVertical) {
      to_x = (int)rays[x].wallHitY % TILE_SIZE;
    } else {
      to_x = (int)rays[x].wallHitX % TILE_SIZE;
    }

    int text_n = rays[x].wallHitContent - 1;

    for (int y = wall_top_pixel; y < wall_bottom_pixel; y++) {
      int distance_from_top = y + (wall_strip_height / 2) - (WINDOW_HEIGHT / 2);
      int to_y = distance_from_top * ((float)get_world_texture(text_n).height / wall_strip_height);

      uint32_t texel_color = get_world_texture(text_n).texture_buffer[(get_world_texture(text_n).width * to_y) + to_x];
      draw_pixel_to_buffer(x, y, texel_color);
    }

    for (int y = wall_bottom_pixel; y < WINDOW_HEIGHT; y++) {
      draw_pixel_to_buffer(x, y, 0xFF777777);
    }
  }
}

void render() {
  clear_color_buffer(0xFF000000);
  generate_projection();

  render_map();
  //render_rays();
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
    return 1;
  }

  if (!load_background_audio(AC_AOT_RAGE)) {
    printf("There was an error loading audio buffer.\n");
    return 1;
  }

  play_background_audio(-1);

  setup();

  while (game_running) {
    process_input();
    update();
    render();
  }

  free_resources();

  return 0;
}
