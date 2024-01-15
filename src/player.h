#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
  float   x;
  float   y;
  float   width;
  float   height;
  int     turnDirection;
  int     walkDirection;
  float   rotationAngle;
  float   walkSpeed;
  float   turnSpeed;
} player_t;

extern player_t player;

void render_player(void);
void move_player(float delta_time);
void render_player_angle(void);

#endif // !PLAYER_H
