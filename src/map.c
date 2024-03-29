#include "defs.h"
#include "map.h"
#include "stdbool.h"
#include "graphics.h"

static const int MAP[MAP_NUM_ROWS][MAP_NUM_COLS] = {
  {17, 17, 16, 16, 16, 15, 15, 17, 17, 17, 15, 15, 15 ,15, 15, 15, 15, 15, 15, 15},
  {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 15},
  {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15},
  {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15},
  {17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15},
  {17, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15},
  {17, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 15},
  {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 0, 16, 0, 0, 0, 0, 0, 15},
  {15, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15},
  {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 16},
  {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 16},
  {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 16},
  {16, 16, 15, 15, 15, 15, 15, 16, 16, 16, 17, 17, 16, 15, 16, 16, 16, 16, 16, 16}
};

bool vector_contains_wall(float x, float y) {
  if (x < 0 || x > MAP_NUM_COLS * TILE_SIZE || y < 0 || y > MAP_NUM_ROWS * TILE_SIZE)
    return true;
  int map_x = floor(x / TILE_SIZE);
  int map_y = floor(y / TILE_SIZE);
  return MAP[map_y][map_x] != 0;
}

int get_map_content(int i, int j) {
  return MAP[i][j];
}

bool vector_within_map(float x, float y) {
  return x >= 0 && x <= MAP_NUM_COLS * TILE_SIZE && y >= 0 && y <= MAP_NUM_ROWS * TILE_SIZE;
}

void render_map() {
 for (int i = 0; i < MAP_NUM_ROWS; i++) {
      for (int j = 0; j < MAP_NUM_COLS; j++) {
        int tileX = j * TILE_SIZE;
        int tileY = i * TILE_SIZE;
        uint32_t tileColor = MAP[i][j] != 0 ? 0xFFFFFFFF : 0x00000000;
        draw_rect_to_buffer(
            tileX * MINIMAP_SCALE_FACTOR,
            tileY * MINIMAP_SCALE_FACTOR,
            TILE_SIZE * MINIMAP_SCALE_FACTOR,
            TILE_SIZE * MINIMAP_SCALE_FACTOR,
            tileColor
        );
      }
  }
}
