#ifndef SPRITE_H
#define SPRITE_H

#define NUM_SPRITES 1

typedef struct {
  float x;
  float y;
  float distance;
  float angle;
  int texture;
} sprite_t;

void generate_sprite_projection(void);
void render_map_sprites(void);

#endif
