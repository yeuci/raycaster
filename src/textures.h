#ifndef TEXTURES_H
#define TEXTURES_H

#include "defs.h"
#include "stdint.h"
#include "upng.h"

#define NUM_TEXTURES        17

upng_t* get_world_texture(int idx);
void load_world_textures();
void free_world_textures();
void change_color_intensity(uint32_t* color, float factor);

#endif
