#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "sprites.h"
#include "boss.h"           
#include "enemy_bullet.h"
#include "bonus_ship.h"
#include "shields.h"
#include "game_events.h"

typedef enum {
    GAME_STATE_PLAYING,
    GAME_STATE_WIN,
    GAME_STATE_LOSE,
    GAME_STATE_MENU
} GameState;

static inline int enemy_score_for_type(int type) {
    switch (type) {
        case 0:  return 30;
        case 1:  return 20;
        case 2:  return 10;
        default: return 5;
    }
}

typedef struct {
    Player          player;
    EnemyGrid       enemies;
    BulletPool      bullets;
    EnemyBulletPool enemy_bullets;
    Boss            boss;
    Shields         shields;
    BonusShip       bonus_ship;      
    bool            is_boss_level; 
    GameSprites     sprites;
    GameState       state;
    int             score;
    int             level;
    float           win_timer;
    float           enemy_shoot_timer;
    int             screen_width;
    int             screen_height;
    GameEventQueue  events;
} Game;

void game_reset(Game *game, int screen_width, int screen_height);
void game_update(Game *game, float dt);
void game_draw(Game *game, float dt);
bool game_menu_handle_input(Game *game);
void game_next_level(Game *game);

#endif // GAME_H