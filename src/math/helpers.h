#ifndef HELPERS_H
#define HELPERS_H

#include <math.h>

float normalize_angle(float angle);
float vector_distance(float x_1, float y_1, float x_2, float y_2);
bool vector_contains_wall(float x, float y);

#endif
