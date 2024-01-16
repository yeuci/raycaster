#include "sprite.h"
#include "textures.h"
#include "graphics.h"
#include "player.h"



static sprite_t sprites[NUM_SPRITES] = {
  { .x = 640, .y = 630, .texture = 9 }
};

void generate_sprite_projection(void) {
  //
}

void render_map_sprites(void) {
 for (int i = 0; i < NUM_SPRITES; i++) {
    draw_rect_to_buffer(
      sprites[i].x * MINIMAP_SCALE_FACTOR,
      sprites[i].y * MINIMAP_SCALE_FACTOR,
      2,
      2,
      0xFFFFFFFF);
  }
}
