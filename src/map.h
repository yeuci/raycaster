#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#define MAP_NUM_ROWS        13
#define MAP_NUM_COLS        20

bool vector_contains_wall(float x, float y);
void render_map();
int get_map_content(int i, int j);
bool vector_within_map(float x, float y);

#endif // !MAP_H
