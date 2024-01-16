#include "sprite.h"
#include "textures.h"
#include "graphics.h"
#include "player.h"
#include <math.h>
#include "defs.h"
#include "ray.h"


static sprite_t sprites[NUM_SPRITES] = {
  { .x = 640, .y = 630, .texture = 9  },
  { .x = 250, .y = 600, .texture = 11 },
  { .x = 250, .y = 600, .texture = 10 },
  { .x = 300, .y = 400, .texture = 12 },
  { .x = 90, .y = 100, .texture = 13 }
};

void generate_sprite_projection(void) {
  sprite_t visible_sprites[NUM_SPRITES];
  int num_visible_sprites = 0;

  for (int i = 0; i < NUM_SPRITES; i++) {
    float angle_sprite_to_player = player.rotationAngle - atan2(sprites[i].y - player.y, sprites[i].x - player.x);

    if (angle_sprite_to_player > PI)
      angle_sprite_to_player -= TWO_PI;
    if (angle_sprite_to_player < -PI)
      angle_sprite_to_player += TWO_PI;
    angle_sprite_to_player = fabs(angle_sprite_to_player);

    const float EPSILON = 0.2;
    if (angle_sprite_to_player < (FOV_ANGLE / 2) + EPSILON) {
      sprites[i].visible = true;
      sprites[i].angle = angle_sprite_to_player;
      sprites[i].distance = vector_distance(sprites[i].x, sprites[i].y, player.x, player.y);
      visible_sprites[num_visible_sprites] = sprites[i];
      num_visible_sprites++;
    } else {
      sprites[i].visible = false;
    }
  }

  for (int i = 0; i < num_visible_sprites - 1; i++) {
    for (int j = i + 1; j < num_visible_sprites; j++) {
      if (visible_sprites[i].distance < visible_sprites[j].distance) {
        sprite_t temp = visible_sprites[i];
        visible_sprites[i] = visible_sprites[j];
        visible_sprites[j] = temp;
      }
    }
  }

  for (int i = 0; i < num_visible_sprites; i++) {
    sprite_t sprite = visible_sprites[i];

    float perp_distance = sprite.distance * cos(sprite.angle);
    float sprite_height = (TILE_SIZE / perp_distance) * DIST_PROJ_PLANE;
    float sprite_width = sprite_height;

    float sprite_top_y = (WINDOW_HEIGHT / 2) - (sprite_height / 2);
    sprite_top_y = (sprite_top_y < 0) ? 0 : sprite_top_y;

    float sprite_bottom_y = (WINDOW_HEIGHT / 2) + (sprite_height / 2);
    sprite_bottom_y = (sprite_bottom_y > WINDOW_HEIGHT) ? WINDOW_HEIGHT : sprite_bottom_y;

    float sprite_angle = atan2(sprite.y - player.y, sprite.x - player.x) - player.rotationAngle;
    float sprite_screen_pos_x = tan(sprite_angle) * DIST_PROJ_PLANE;

    float sprite_left_x = (WINDOW_WIDTH / 2) + sprite_screen_pos_x - (sprite_width / 2);
    float sprite_right_x = sprite_left_x + sprite_width;

    int texture_width = upng_get_width(get_world_texture(sprite.texture));
    int texture_height = upng_get_width(get_world_texture(sprite.texture));

    for (int x = sprite_left_x; x < sprite_right_x; x++) {
      float texel_width = (texture_width / sprite_width);
      int to_x = (x - sprite_left_x) * texel_width;
      for (int y = sprite_top_y; y < sprite_bottom_y; y++) {
        if (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT) {
          int distance_from_top = y + (sprite_height / 2) - (WINDOW_HEIGHT / 2);
          int to_y = distance_from_top * (texture_height / sprite_height);

          uint32_t* sprite_texture_buffer = (uint32_t*)upng_get_buffer(get_world_texture(sprite.texture));
          color_t texel_color = sprite_texture_buffer[(texture_width * to_y) + to_x];
          if (sprite.distance < rays[x].distance && texel_color != 0xFFFF00FF) {
            draw_pixel_to_buffer(x, y, texel_color);
          }
        }
      }
    }

  }

}

void render_map_sprites(void) {
 for (int i = 0; i < NUM_SPRITES; i++) {
    draw_rect_to_buffer(
      sprites[i].x * MINIMAP_SCALE_FACTOR,
      sprites[i].y * MINIMAP_SCALE_FACTOR,
      2,
      2,
      sprites[i].visible ? 0xFF00FF00 : 0xFFFFFFFF);
  }
}
