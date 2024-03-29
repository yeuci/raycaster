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
void draw_rect_to_buffer(int x, int y, int width, int height, uint32_t color);
void draw_line_to_buffer(int x_0, int x_1, int y_0, int y_1, uint32_t color);

SDL_Renderer* get_renderer(void);

#endif
