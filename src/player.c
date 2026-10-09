#include "player.h"

void player_init(Player *player, int screen_width, int screen_height) {
    player->position.x = (screen_width  - PLAYER_WIDTH)  / 2.0f;
    player->position.y = screen_height - PLAYER_HEIGHT - PLAYER_BOTTOM_MARGIN;
    player->speed = PLAYER_SPEED;
    player->alive = true;
}

void player_update(Player *player, float dt, int screen_width) {
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        player->position.x -= player->speed * dt;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        player->position.x += player->speed * dt;
    }

    if (player->position.x < 0) {
        player->position.x = 0;
    }
    if (player->position.x + PLAYER_WIDTH > screen_width) {
        player->position.x = screen_width - PLAYER_WIDTH;
    }
}

void player_draw(const Player *player) {
    DrawRectangle(
        (int)player->position.x,
        (int)player->position.y,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        BLUE
    );
}