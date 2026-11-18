
#include "shields.h"

void shields_init(
    Shields *shields,
    int screen_width,
    int screen_height
) {
    float gap =
        (screen_width - SHIELD_COUNT * SHIELD_WIDTH) /
        (float)(SHIELD_COUNT + 1);

    float top = (float)screen_height - 115.0f;

    for (int s = 0; s < SHIELD_COUNT; s++) {
        float left = gap * (s + 1) + s * SHIELD_WIDTH;

        for (int row = 0; row < SHIELD_ROWS; row++) {
            for (int col = 0; col < SHIELD_COLS; col++) {
                ShieldTile *tile =
                    &shields->tiles[s][row][col];

                tile->rect = (Rectangle){
                    left + col * SHIELD_TILE_SIZE,
                    top + row * SHIELD_TILE_SIZE,
                    SHIELD_TILE_SIZE,
                    SHIELD_TILE_SIZE
                };

                bool hole =
                    (row == 0 && (col == 0 || col == 5)) ||
                    (row == 2 && (col == 2 || col == 3));

                tile->hp = hole ? 0 : SHIELD_MAX_HP;
            }
        }
    }
}


bool shields_hit(Shields *shields, Rectangle bullet_rect) {
    for (int s = 0; s < SHIELD_COUNT; s++) {
        for (int row = 0; row < SHIELD_ROWS; row++) {
            for (int col = 0; col < SHIELD_COLS; col++) {
                ShieldTile *tile =
                    &shields->tiles[s][row][col];

                if (tile->hp <= 0) continue;

                if (CheckCollisionRecs(tile->rect, bullet_rect)) {
                    tile->hp--;
                    return true;
                }
            }
        }
    }

    return false;
}

void shields_draw(const Shields *shields) {
    for (int s = 0; s < SHIELD_COUNT; s++) {
        for (int row = 0; row < SHIELD_ROWS; row++) {
            for (int col = 0; col < SHIELD_COLS; col++) {
                const ShieldTile *tile =
                    &shields->tiles[s][row][col];

                if (tile->hp <= 0) continue;

                Color color = tile->hp == 3
                    ? GREEN
                    : (tile->hp == 2 ? YELLOW : ORANGE);

                DrawRectangle(
                    (int)tile->rect.x,
                    (int)tile->rect.y,
                    SHIELD_TILE_SIZE - 1,
                    SHIELD_TILE_SIZE - 1,
                    color
                );
            }
        }
    }
}
