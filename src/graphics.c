#include "defs.h"
#include "textures.h"
#include "sound_manager.h"
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

static SDL_Window*   window              = NULL;
static SDL_Renderer* renderer            = NULL;

static uint32_t* color_buffer              = NULL;
static SDL_Texture* color_buffer_texture = NULL;


bool initialize_window() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  SDL_DisplayMode display_mode_buffer;
  SDL_GetCurrentDisplayMode(0, &display_mode_buffer);

  window = SDL_CreateWindow(
    "Raycast",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    display_mode_buffer.w,
    display_mode_buffer.h,
    SDL_WINDOW_BORDERLESS
  );

  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * (uint32_t)WINDOW_WIDTH * (uint32_t)WINDOW_HEIGHT);

  if (!color_buffer) {
    fprintf(stderr, "Error creating allocating memory for color buffer.\n");
    return false;
  }

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

  return true;
}

SDL_Renderer* get_renderer(void) {
  return renderer;
}

void destroy_window() {
  SDL_DestroyTexture(color_buffer_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  free(color_buffer);
}

void clear_color_buffer(uint32_t color) {
  for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
    color_buffer[i] = color;
  }
}

void render_color_buffer() {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(WINDOW_WIDTH * sizeof(uint32_t)));
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void draw_pixel_to_buffer(int x, int y, uint32_t color) {
  color_buffer[(WINDOW_WIDTH * y) + x] = color;
}
