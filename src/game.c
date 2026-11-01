#include "game.h"
#include <raylib.h>
#include "sprites.h"

GameSprites sprites;

void game_reset(Game *game, int screen_width, int screen_height) {
    game->screen_width = screen_width;
    game->screen_height = screen_height;
    game->state = GAME_STATE_PLAYING;
    game->score = 0;
    game->level = 1;          
    game->win_timer = 0.0f;   

    player_init(&game->player, screen_width, screen_height, game->sprites.player);
    enemy_grid_init(&game->enemies, game->level, game->sprites.enemy_squid, game->sprites.enemy_crab, game->sprites.enemy_octopus);
    bullet_pool_init(&game->bullets, game->sprites.bullet);
    eb_pool_init(&game->enemy_bullets);

    game->is_boss_level = (game->level % 5 == 0); 

    if (game->is_boss_level) {
        boss_init(&game->boss, screen_width);
    } else {
        enemy_grid_init(&game->enemies, game->level, game->sprites.enemy_squid, game->sprites.enemy_crab, game->sprites.enemy_octopus);
    }
    
    bullet_pool_init(&game->bullets, game->sprites.bullet);
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
    if (game->state == GAME_STATE_LOSE) return;

    if (game->state == GAME_STATE_WIN) {
        game->win_timer += dt;
        if (game->win_timer >= 2.0f) {
            game_next_level(game);
        }
        return;
    }

    player_update(&game->player, dt, game->screen_width);
    bullet_pool_update(&game->bullets, dt);
    eb_pool_update(&game->enemy_bullets, dt, game->screen_height);

    if (game->is_boss_level) {
        boss_update(&game->boss, dt, game->screen_width, game->screen_height);
        
        if (game->boss.shoot_timer >= 1.5f && game->boss.alive) {
            eb_pool_spawn(&game->enemy_bullets, game->boss.position);
            game->boss.shoot_timer = 0.0f;
        }

        Rectangle boss_rect = { game->boss.position.x, game->boss.position.y, BOSS_WIDTH, BOSS_HEIGHT };
        if (bullet_pool_check_hit(&game->bullets, boss_rect)) {
            game->boss.hp--;
            if (game->boss.hp <= 0) {
                game->boss.alive = false;
                game->state = GAME_STATE_WIN;
                return;
            }
        }
    } else {
        enemy_grid_update(&game->enemies, dt, game->screen_width);
        handle_bullet_collisions(game);
        
        if (enemy_grid_is_empty(&game->enemies)) {
            game->state = GAME_STATE_WIN;
            return;
        }
    }

    Rectangle player_rect = { game->player.position.x, game->player.position.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    if (eb_pool_check_hit_player(&game->enemy_bullets, player_rect)) {
        player_take_damage(&game->player);
        if (!game->player.alive) {
            game->state = GAME_STATE_LOSE;
            return;
        }
    }

    if (!game->is_boss_level) {
        int player_top = (int)game->player.position.y;
        if (enemy_grid_reached_bottom(&game->enemies, player_top)) {
            game->state = GAME_STATE_LOSE;
            return;
        }
    }
}

static void draw_centered_text(const char *text, int y, int size, Color color, int screen_width) {
    int width = MeasureText(text, size);
    DrawText(text, (screen_width - width) / 2, y, size, color);
}

void game_draw(Game *game, float dt) {
    ClearBackground((Color){ 10, 10, 25, 255 });
    
    sprites_update_stars(&game->sprites, dt);
    sprites_draw_stars(&game->sprites, game->screen_width, game->screen_height);

    player_draw(&game->player);
    enemy_grid_draw(&game->enemies);
    bullet_pool_draw(&game->bullets);
    eb_pool_draw(&game->enemy_bullets);

    DrawFPS(10, 10);
    DrawText(TextFormat("Score: %d", game->score), 10, 30, 20, LIGHTGRAY);
    DrawText(TextFormat("Level: %d", game->level), 10, 55, 20, LIGHTGRAY);
    DrawText("Arrows/A,D move | Space shoot | R restart", 10, 80, 14, GRAY);

    if (game->state == GAME_STATE_WIN) {
        draw_centered_text("LEVEL CLEARED!", 180, 50, GREEN, game->screen_width);
        draw_centered_text("Get ready for the next level...", 250, 20, LIME, game->screen_width);
    } else if (game->state == GAME_STATE_LOSE) {
        draw_centered_text("GAME OVER", 180, 50, RED, game->screen_width);
        draw_centered_text(TextFormat("You reached level %d", game->level), 230, 20, ORANGE, game->screen_width);
        draw_centered_text("Press R to play again", 260, 20, LIGHTGRAY, game->screen_width);
    }
    if (game->is_boss_level) {
        boss_draw(&game->boss);
    } else {
        enemy_grid_draw(&game->enemies);
    }

    for (int i = 0; i < game->player.max_hp; i++) {
        Color c = (i < game->player.hp) ? RED : DARKGRAY;
        DrawRectangle(10 + i * 20, 105, 15, 15, c);
    }
}

void game_next_level(Game *game) {
    game->level++;
    game->state = GAME_STATE_PLAYING;
    game->win_timer = 0.0f;
    
    player_init(&game->player, game->screen_width, game->screen_height, game->sprites.player);
    eb_pool_init(&game->enemy_bullets);
    bullet_pool_init(&game->bullets, game->sprites.bullet);
    
    game->is_boss_level = (game->level % 5 == 0);
    
    if (game->is_boss_level) {
        boss_init(&game->boss, game->screen_width);
    } else {
        enemy_grid_init(&game->enemies, game->level, game->sprites.enemy_squid, game->sprites.enemy_crab, game->sprites.enemy_octopus);
    }
}