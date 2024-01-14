#include "ray.h"
#include <math.h>
#include "player.h"
#include "map.h"
#include <float.h>

ray_t rays[NUM_RAYS];

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

  while (vector_within_map(nextHorzTouchX, nextHorzTouchY)) {
    float xToCheck = nextHorzTouchX;
    float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

    if (vector_contains_wall(xToCheck, yToCheck)) {
      horzWallHitX = nextHorzTouchX;
      horzWallHitY = nextHorzTouchY;
      horzWallContent = get_map_content((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
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

  while (vector_within_map(nextVertTouchX, nextVertTouchY)) {
    float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
    float yToCheck = nextVertTouchY;

    if (vector_contains_wall(xToCheck, yToCheck)) {
      vertWallHitX = nextVertTouchX;
      vertWallHitY = nextVertTouchY;
      vertWallContent = get_map_content((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
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
