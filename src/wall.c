#include "defs.h"
#include "graphics.h"
#include "ray.h"
#include "player.h"
#include <math.h>
#include "textures.h"

void generate_projection(void) {
  for (int x = 0; x < NUM_RAYS; x++) {
    float p_distance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);
    float projected_wall_height = (TILE_SIZE / p_distance) * DIST_PROJ_PLANE;

    int wall_strip_height = (int)projected_wall_height;

    int wall_top_pixel = (WINDOW_HEIGHT / 2) - (wall_strip_height / 2);
    wall_top_pixel = wall_top_pixel < 0 ? 0 : wall_top_pixel;

    int wall_bottom_pixel = (WINDOW_HEIGHT / 2) + (wall_strip_height / 2);
    wall_bottom_pixel = wall_bottom_pixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wall_bottom_pixel;

    for (int y = 0; y < wall_top_pixel; y++) {
      draw_pixel_to_buffer(x, y, 0xFF333333);
    }

    int to_x;
    if (rays[x].wasHitVertical) {
      to_x = (int)rays[x].wallHitY % TILE_SIZE;
    } else {
      to_x = (int)rays[x].wallHitX % TILE_SIZE;
    }

    int text_n = rays[x].wallHitContent - 1;

    for (int y = wall_top_pixel; y < wall_bottom_pixel; y++) {
      int distance_from_top = y + (wall_strip_height / 2) - (WINDOW_HEIGHT / 2);
      int to_y = distance_from_top * ((float)get_world_texture(text_n).height / wall_strip_height);

      uint32_t texel_color = get_world_texture(text_n).texture_buffer[(get_world_texture(text_n).width * to_y) + to_x];
      draw_pixel_to_buffer(x, y, texel_color);
    }

    for (int y = wall_bottom_pixel; y < WINDOW_HEIGHT; y++) {
      draw_pixel_to_buffer(x, y, 0xFF777777);
    }
  }
}
