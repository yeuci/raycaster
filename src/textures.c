#include "textures.h"
#include <stdio.h>
#include <stdint.h>

static const char* TEXTURE_FILE_NAMES[NUM_TEXTURES] = {
  "./assets/images/redbrick.png",
  "./assets/images/purplestone.png",
  "./assets/images/mossystone.png",
  "./assets/images/graystone.png",
  "./assets/images/colorstone.png",
  "./assets/images/bluestone.png",
  "./assets/images/wood.png",
  "./assets/images/eagle.png",
  "./assets/images/pikuma.png"
};

texture_t world_textures[NUM_TEXTURES];

void load_world_textures() {
  for (int i = 0; i < NUM_TEXTURES; i++) {
    upng_t* upng;

    upng = upng_new_from_file(TEXTURE_FILE_NAMES[i]);
    if (upng != NULL) {
      upng_decode(upng);
      if (upng_get_error(upng) == UPNG_EOK) {
        world_textures[i].upng_texture = upng;
        world_textures[i].width = upng_get_width(upng);
        world_textures[i].height = upng_get_height(upng);
        world_textures[i].texture_buffer = (uint32_t*)upng_get_buffer(upng);
      }
    }
  }
}

void free_world_textures() {
  for (int i = 0; i < NUM_TEXTURES; i++) {
    upng_free(world_textures[i].upng_texture);
  }
}

texture_t get_world_texture(int idx) {
  return world_textures[idx];
}
