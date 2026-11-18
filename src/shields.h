
#ifndef SHIELDS_H
#define SHIELDS_H

#include <raylib.h>

#define SHIELD_COUNT 4
#define SHIELD_ROWS 3
#define SHIELD_COLS 6
#define SHIELD_TILE_SIZE 10
#define SHIELD_WIDTH (SHIELD_COLS * SHIELD_TILE_SIZE)
#define SHIELD_MAX_HP 3

typedef struct {
    Rectangle rect;
    int hp;
} ShieldTile;

typedef struct {
    ShieldTile tiles[SHIELD_COUNT][SHIELD_ROWS][SHIELD_COLS];
} Shields;

void shields_init(Shields *shields, int screen_width, int screen_height);
void shields_draw(const Shields *shields);
bool shields_hit(Shields *shields, Rectangle bullet_rect);

#endif
