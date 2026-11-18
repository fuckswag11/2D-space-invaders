
#ifndef BONUS_SHIP_H
#define BONUS_SHIP_H

#include <raylib.h>

#define BONUS_SHIP_WIDTH 48
#define BONUS_SHIP_HEIGHT 20
#define BONUS_SHIP_Y 20.0f
#define BONUS_SHIP_SPEED 140.0f
#define BONUS_SHIP_INTERVAL 10.0f
#define BONUS_SHIP_SCORE 100

typedef struct {
    Vector2 position;
    bool active;
    float spawn_timer;
} BonusShip;

void bonus_ship_init(BonusShip *ship);
void bonus_ship_update(BonusShip *ship, float dt, int screen_width);
void bonus_ship_draw(const BonusShip *ship);

#endif
