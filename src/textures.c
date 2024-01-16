#include "textures.h"
#include <stdio.h>
#include <stdint.h>
#include "upng.h"

static const char* TEXTURE_FILE_NAMES[NUM_TEXTURES] = {
  "./assets/images/redbrick.png",    // [0]
  "./assets/images/purplestone.png", // [1]
  "./assets/images/mossystone.png",  // [2]
  "./assets/images/graystone.png",   // [3]
  "./assets/images/colorstone.png",  // [4]
  "./assets/images/bluestone.png",   // [5]
  "./assets/images/wood.png",        // [6]
  "./assets/images/eagle.png",       // [7]
  "./assets/images/pikuma.png",      // [8]
  "./assets/images/barrel.png",      // [9]
  "./assets/images/light.png",       // [10]
  "./assets/images/table.png",       // [11]
  "./assets/images/guard.png",       // [12]
  "./assets/images/armor.png"        // [13]
};

upng_t* world_textures[NUM_TEXTURES];

void load_world_textures() {
  for (int i = 0; i < NUM_TEXTURES; i++) {
    upng_t* upng = upng_new_from_file(TEXTURE_FILE_NAMES[i]);
    if (upng != NULL) {
      upng_decode(upng);
      if (upng_get_error(upng) == UPNG_EOK) {
        world_textures[i] = upng;
      } else {
        printf("Error decoding texture %s\n", TEXTURE_FILE_NAMES[i]);
      }
    } else {
      printf("Error loading texture %s\n", TEXTURE_FILE_NAMES[i]);
    }
  }
}

void free_world_textures() {
  for (int i = 0; i < NUM_TEXTURES; i++) {
    upng_free(world_textures[i]);
  }
}

upng_t* get_world_texture(int idx) {
  return world_textures[idx];
}

void change_color_intensity(uint32_t* color, float factor) {
  color_t a = (*color & 0xFF000000);
  color_t r = (*color & 0x00FF0000) * factor;
  color_t g = (*color & 0x0000FF00) * factor;
  color_t b = (*color & 0x000000FF) * factor;
  *color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}
