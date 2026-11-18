#include "bonus_ship.h"

void bonus_ship_init(BonusShip *ship) {
    ship->position = (Vector2){
        -BONUS_SHIP_WIDTH,
        BONUS_SHIP_Y
    };

    ship->active = false;
    ship->spawn_timer = 0.0f;
}

void bonus_ship_update(
    BonusShip *ship,
    float dt,
    int screen_width
) {
    if (!ship->active) {
        ship->spawn_timer += dt;

        if (ship->spawn_timer >= BONUS_SHIP_INTERVAL) {
            ship->spawn_timer = 0.0f;
            ship->active = true;
            ship->position.x = -BONUS_SHIP_WIDTH;
            ship->position.y = BONUS_SHIP_Y;
        }

        return;
    }

    ship->position.x += BONUS_SHIP_SPEED * dt;

    if (ship->position.x > screen_width) {
        ship->active = false;
        ship->spawn_timer = 0.0f;
    }
}

void bonus_ship_draw(const BonusShip *ship) {
    if (!ship->active) return;

    int x = (int)ship->position.x;
    int y = (int)ship->position.y;

    DrawRectangle(x + 12, y,      24, 5, ORANGE);
    DrawRectangle(x + 6,  y + 5,  36, 8, RED);
    DrawRectangle(x,      y + 11, 48, 6, MAROON);
    DrawRectangle(x + 10, y + 17,  7, 3, YELLOW);
    DrawRectangle(x + 31, y + 17,  7, 3, YELLOW);
}
