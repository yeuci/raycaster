#ifndef TEXTURES_H
#define TEXTURES_H

#include "defs.h"
#include "stdint.h"
#include "upng.h"

typedef struct {
  upng_t* upng_texture;
  int width;
  int height;
  uint32_t* texture_buffer;
} texture_t;

texture_t get_world_texture(int idx);

void load_world_textures();
void free_world_textures();

#endif
