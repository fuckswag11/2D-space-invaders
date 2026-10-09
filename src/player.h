#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define PLAYER_WIDTH   40
#define PLAYER_HEIGHT  20

#define PLAYER_SPEED   300.0f

#define PLAYER_BOTTOM_MARGIN 30

typedef struct {
    Vector2 position;   
    float   speed;   
    bool    alive;   
} Player;

void player_init(Player *player, int screen_width, int screen_height);


void player_update(Player *player, float dt, int screen_width);


void player_draw(const Player *player);

#endif 