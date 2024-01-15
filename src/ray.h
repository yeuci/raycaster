#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include "defs.h"

typedef struct {
  float rayAngle;
  float wallHitX;
  float wallHitY;
  float distance;
  float wasHitVertical;
  int  wallHitContent;
} ray_t;

extern ray_t rays[NUM_RAYS];

float normalize_angle(float angle);
float vector_distance(float x_1, float y_1, float x_2, float y_2);
void cast_ray(float ray_angle, int strip_id);
void cast_all_rays();
void render_rays();


#endif // !RAY_H
