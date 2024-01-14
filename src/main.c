#include "defs.h"
#include "textures.h"
#include "graphics.h"
#include "audio_constants.h"
#include "audio_manager.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


const int MAP[MAP_NUM_ROWS][MAP_NUM_COLS] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 2, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5}
};

struct Player {
  float   x;
  float   y;
  float   width;
  float   height;
  int     turnDirection;
  int     walkDirection;
  float   rotationAngle;
  float   walkSpeed;
  float   turnSpeed;
} player;

struct Ray {
  float rayAngle;
  float wallHitX;
  float wallHitY;
  float distance;
  float wasHitVertical;
  int  wallHitContent;
} rays[NUM_RAYS];

bool game_running                 = false;

int last_frame_ticks              = 0;

float normalize_angle(float angle) {
  angle = remainder(angle, TWO_PI);
  if (angle < 0) {
    angle = TWO_PI + angle;
  }
  return angle;
}

float vector_distance(float x_1, float y_1, float x_2, float y_2) {
  return sqrt((x_2 - x_1) * (x_2 - x_1) + (y_2 - y_1) * (y_2 - y_1));
}

bool vector_contains_wall(float x, float y) {
  if (x < 0 || x > MAP_NUM_COLS * TILE_SIZE || y < 0 || y > MAP_NUM_ROWS * TILE_SIZE)
    return true;
  return MAP[(int)floor(y / TILE_SIZE)][(int)floor(x / TILE_SIZE)] != 0;
}


void cast_ray(float ray_angle, int strip_id) {
  ray_angle = normalize_angle(ray_angle);

  bool isRayFacingDown = ray_angle > 0 && ray_angle < PI;
  bool isRayFacingUp = !isRayFacingDown;
  bool isRayFacingRight = ray_angle < 0.5 * PI || ray_angle > 1.5 * PI;
  bool isRayFacingLeft = !isRayFacingRight;


  float xintercept, yintercept;
  float xstep, ystep;

  bool foundHorzWallHit = false;
  float horzWallHitX = 0;
  float horzWallHitY = 0;
  int horzWallContent = 0;

  yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
  yintercept += isRayFacingDown ? TILE_SIZE : 0;

  xintercept = player.x + (yintercept - player.y) / tan(ray_angle);

  ystep = TILE_SIZE;
  ystep *= isRayFacingUp ? -1 : 1;

  xstep = TILE_SIZE / tan(ray_angle);
  xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
  xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

  float nextHorzTouchX = xintercept;
  float nextHorzTouchY = yintercept;

  while (nextHorzTouchX >= 0 && nextHorzTouchX <= MAP_NUM_COLS * TILE_SIZE && nextHorzTouchY >= 0 && nextHorzTouchY <= MAP_NUM_ROWS * TILE_SIZE) {
    float xToCheck = nextHorzTouchX;
    float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

    if (vector_contains_wall(xToCheck, yToCheck)) {
      horzWallHitX = nextHorzTouchX;
      horzWallHitY = nextHorzTouchY;
      horzWallContent = MAP[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
      foundHorzWallHit = true;
      break;
    } else {
      nextHorzTouchX += xstep;
      nextHorzTouchY += ystep;
    }
  }

  bool foundVertWallHit = false;
  float vertWallHitX = 0;
  float vertWallHitY = 0;
  int vertWallContent = 0;

  xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
  xintercept += isRayFacingRight ? TILE_SIZE : 0;

  yintercept = player.y + (xintercept - player.x) * tan(ray_angle);

  xstep = TILE_SIZE;
  xstep *= isRayFacingLeft ? -1 : 1;

  ystep = TILE_SIZE * tan(ray_angle);
  ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
  ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

  float nextVertTouchX = xintercept;
  float nextVertTouchY = yintercept;

  while (nextVertTouchX >= 0 && nextVertTouchX <= MAP_NUM_COLS * TILE_SIZE && nextVertTouchY >= 0 && nextVertTouchY <= MAP_NUM_ROWS * TILE_SIZE) {
    float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
    float yToCheck = nextVertTouchY;

    if (vector_contains_wall(xToCheck, yToCheck)) {
      vertWallHitX = nextVertTouchX;
      vertWallHitY = nextVertTouchY;
      vertWallContent = MAP[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
      foundVertWallHit = true;
      break;
    } else {
      nextVertTouchX += xstep;
      nextVertTouchY += ystep;
    }
  }

  float horzHitDistance = foundHorzWallHit ? vector_distance(player.x, player.y, horzWallHitX, horzWallHitY) : FLT_MAX;
  float vertHitDistance = foundVertWallHit ? vector_distance(player.x, player.y, vertWallHitX, vertWallHitY) : FLT_MAX;

  if (vertHitDistance < horzHitDistance) {
    rays[strip_id].distance = vertHitDistance;
    rays[strip_id].wallHitX = vertWallHitX;
    rays[strip_id].wallHitY = vertWallHitY;
    rays[strip_id].wallHitContent = vertWallContent;
    rays[strip_id].wasHitVertical = true;
  } else {
    rays[strip_id].distance = horzHitDistance;
    rays[strip_id].wallHitX = horzWallHitX;
    rays[strip_id].wallHitY = horzWallHitY;
    rays[strip_id].wallHitContent = horzWallContent;
    rays[strip_id].wasHitVertical = false;
  }

  rays[strip_id].rayAngle = ray_angle;
}

void cast_all_rays() {
  for (int col = 0; col < NUM_RAYS; col++) {
    float ray_angle = player.rotationAngle + atan((col - NUM_RAYS / 2) / DIST_PROJ_PLANE);
    cast_ray(ray_angle, col);
  }
}

void render_map() {
 /* for (int i = 0; i < MAP_NUM_ROWS; i++) {
      for (int j = 0; j < MAP_NUM_COLS; j++) {
        int tileX = j * TILE_SIZE;
        int tileY = i * TILE_SIZE;
        int tileColor = MAP[i][j] != 0 ? 255 : 0;

        SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
        SDL_Rect mapTileRect = {
            tileX * MINIMAP_SCALE_FACTOR,
            tileY * MINIMAP_SCALE_FACTOR,
            TILE_SIZE * MINIMAP_SCALE_FACTOR,
            TILE_SIZE * MINIMAP_SCALE_FACTOR
        };
        SDL_RenderFillRect(renderer, &mapTileRect);
      }
  } */
}

void render_rays() {
  /*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (int i = 0; i < NUM_RAYS; i++) {
    SDL_RenderDrawLine(
      renderer,
      MINIMAP_SCALE_FACTOR * player.x,
      MINIMAP_SCALE_FACTOR * player.y,
      MINIMAP_SCALE_FACTOR * rays[i].wallHitX,
      MINIMAP_SCALE_FACTOR * rays[i].wallHitY
    );
  }
  */
}

void process_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT: {
        game_running = false;
        break;
      }

      case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          game_running = false;
        }
        if (event.key.keysym.sym == SDLK_r) {
          load_background_audio(AC_AOT_RAGE);
          play_background_audio(-1);
        }
        if (event.key.keysym.sym == SDLK_t) {
          load_background_audio(AC_RAE_LGC);
          play_background_audio(-1);
        }
        if (event.key.keysym.sym == SDLK_UP) {
          player.walkDirection = 1;
        }
        if (event.key.keysym.sym == SDLK_DOWN) {
          player.walkDirection = -1;
        }
        if (event.key.keysym.sym == SDLK_RIGHT) {
          player.turnDirection = 1;
        }
        if (event.key.keysym.sym == SDLK_LEFT) {
          player.turnDirection = -1;
        }
        break;
      }

      case SDL_KEYUP: {
        if (event.key.keysym.sym == SDLK_UP) {
          player.walkDirection = 0;
        }
        if (event.key.keysym.sym == SDLK_DOWN) {
          player.walkDirection = 0;
        }
        if (event.key.keysym.sym == SDLK_RIGHT) {
          player.turnDirection = 0;
        }
        if (event.key.keysym.sym == SDLK_LEFT) {
          player.turnDirection = 0;
        }
        break;
      }
    }
  }
}

void setup() {
  player.x             =  WINDOW_WIDTH / 2;
  player.y             =  WINDOW_HEIGHT / 2;
  player.width         = 1;
  player.height        = 1;
  player.turnDirection = 0;
  player.walkDirection = 0;
  player.rotationAngle = PI / 2;
  player.walkSpeed     = 200;
  player.turnSpeed     = 110 * (PI / 180);

  load_world_textures();
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
  /* SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect player_rect = {
    player.x      * MINIMAP_SCALE_FACTOR,
    player.y      * MINIMAP_SCALE_FACTOR,
    player.width  * MINIMAP_SCALE_FACTOR,
    player.height * MINIMAP_SCALE_FACTOR
  };

  SDL_RenderFillRect(renderer, &player_rect);

  render_angle();
  */
}

void update() {
  int time_to_wait = FRAME_TIME_LENGTH - (SDL_GetTicks() - last_frame_ticks);

  if (time_to_wait > 0 && time_to_wait <= FRAME_TIME_LENGTH) {
    SDL_Delay(time_to_wait);
  }

  float delta_time = (SDL_GetTicks() - last_frame_ticks) / 1000.0f;

  last_frame_ticks = SDL_GetTicks();

  move_player(delta_time);
  cast_all_rays();
}

void generate_projection() {
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

void render() {
  clear_color_buffer(0xFF000000);
  generate_projection();

  //render_map();
  //render_rays();
  //render_player();

  render_color_buffer();
}

void free_resources(void) {
  free_world_textures();
  destroy_window();
}

int main(int argc, char** argv) {
  game_running = initialize_window() && initialize_mixer_context();

  if (!game_running) {
    printf("There was an error on initialization.\n");
    return 1;
  }

  if (!load_background_audio(AC_AOT_RAGE)) {
    printf("There was an error loading audio buffer. %s\n");
    return 1;
  }

  play_background_audio(-1);

  setup();

  while (game_running) {
    process_input();
    update();
    render();
  }

  free_resources();

  return 0;
}
