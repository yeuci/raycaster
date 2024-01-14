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

