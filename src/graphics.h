#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool initialize_window(void);

void destroy_window(void);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void draw_pixel_to_buffer(int x, int y, uint32_t color);

SDL_Renderer* get_renderer(void);

#endif
