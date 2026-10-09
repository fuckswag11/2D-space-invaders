#include "game.h"
#include <raylib.h>

void game_reset(Game *game, int screen_width, int screen_height) {
    game->screen_width = screen_width;
    game->screen_height = screen_height;
    game->state = GAME_STATE_PLAYING;
    game->score = 0;

    player_init(&game->player, screen_width, screen_height);
    enemy_grid_init(&game->enemies);
    bullet_pool_init(&game->bullets);
}

static void handle_bullet_collisions(Game *game) {
    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            Enemy *e = &game->enemies.enemies[row][col];
            if (!e->alive) continue;

            Rectangle enemy_rect = {
                e->position.x,
                e->position.y,
                (float)ENEMY_WIDTH,
                (float)ENEMY_HEIGHT
            };

            if (bullet_pool_check_hit(&game->bullets, enemy_rect)) {
                e->alive = false;
                game->enemies.alive_count--;
                game->score += enemy_score_for_type(e->type);
            }
        }
    }
}

void game_update(Game *game, float dt) {
    if (game->state != GAME_STATE_PLAYING) return;

    player_update(&game->player, dt, game->screen_width);
    enemy_grid_update(&game->enemies, dt, game->screen_width);
    bullet_pool_update(&game->bullets, dt);

    handle_bullet_collisions(game);

    if (enemy_grid_is_empty(&game->enemies)) {
        game->state = GAME_STATE_WIN;
        return;
    }

    int player_top = (int)game->player.position.y;
    if (enemy_grid_reached_bottom(&game->enemies, player_top)) {
        game->state = GAME_STATE_LOSE;
        return;
    }
}

static void draw_centered_text(const char *text, int y, int size, Color color, int screen_width) {
    int width = MeasureText(text, size);
    DrawText(text, (screen_width - width) / 2, y, size, color);
}

void game_draw(const Game *game) {
    player_draw(&game->player);
    enemy_grid_draw(&game->enemies);
    bullet_pool_draw(&game->bullets);

    DrawFPS(10, 10);
    DrawText(TextFormat("Score: %d", game->score), 10, 30, 20, DARKGRAY);
    DrawText("Arrows/A,D move | Space shoot | R restart", 10, 55, 14, GRAY);

    if (game->state == GAME_STATE_WIN) {
        draw_centered_text("YOU WIN!", 180, 50, GREEN, game->screen_width);
        draw_centered_text("Press R to play again", 250, 20, DARKGREEN, game->screen_width);
    } else if (game->state == GAME_STATE_LOSE) {
        draw_centered_text("GAME OVER", 180, 50, RED, game->screen_width);
        draw_centered_text("Press R to play again", 250, 20, MAROON, game->screen_width);
    }
}