#include "player.h"
#include "defs.h"
#include "map.h"
#include "graphics.h"
#include "math.h"
#include "map.h"

player_t player = {
  .x             =  WINDOW_WIDTH / 2,
  .y             =  WINDOW_HEIGHT / 2,
  .width         = 1,
  .height        = 1,
  .turnDirection = 0,
  .walkDirection = 0,
  .rotationAngle = PI / 2,
  .walkSpeed     = 200,
  .turnSpeed     = 110 * (PI / 180)
};

void render_angle() {
  /*SDL_RenderDrawLine(
    renderer,
    (player.x + (player.width / 2))    * MINIMAP_SCALE_FACTOR,
    player.y                           * MINIMAP_SCALE_FACTOR,
    ((player.x + (player.width / 2))   + cos(player.rotationAngle) * 40) * MINIMAP_SCALE_FACTOR,
    (player.y                          + sin(player.rotationAngle) * 40) * MINIMAP_SCALE_FACTOR
  );
  */
}

void render_player() {
  draw_rect_to_buffer(
    player.x      * MINIMAP_SCALE_FACTOR,
    player.y      * MINIMAP_SCALE_FACTOR,
    player.width  * MINIMAP_SCALE_FACTOR,
    player.height * MINIMAP_SCALE_FACTOR,
    0xFFFFFFFF
  );

  render_angle();
}

void move_player(float delta_time) {
  player.rotationAngle += player.turnDirection * player.turnSpeed * delta_time;

  float move_step = player.walkDirection * player.walkSpeed * delta_time;

  float new_player_x = player.x + cos(player.rotationAngle) * move_step;
  float new_player_y = player.y + sin(player.rotationAngle) * move_step;

  if (!vector_contains_wall(new_player_x, new_player_y)) {
    player.x = new_player_x;
    player.y = new_player_y;
  }
}
