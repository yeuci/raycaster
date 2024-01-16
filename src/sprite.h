#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>

#define NUM_SPRITES 5

typedef struct {
  float x;
  float y;
  float distance;
  float angle;
  bool visible;
  int texture;
} sprite_t;

void generate_sprite_projection(void);
void render_map_sprites(void);

#endif
