#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define PLAYER_WIDTH   39
#define PLAYER_HEIGHT  24
#define PLAYER_SPEED   300.0f
#define PLAYER_BOTTOM_MARGIN 30
#define PLAYER_MAX_HP  3  

typedef struct {
    Vector2 position;
    float   speed;
    bool    alive;
    int     hp;
    int     max_hp;
    Texture2D sprite;
} Player;

void player_init(Player *player, int screen_width, int screen_height, Texture2D sprite);
void player_update(Player *player, float dt, int screen_width);
void player_draw(const Player *player);
void player_take_damage(Player *player);

#endif