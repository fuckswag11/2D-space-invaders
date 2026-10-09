#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "enemy.h"
#include "bullet.h"

typedef enum {
    GAME_STATE_PLAYING,
    GAME_STATE_WIN,
    GAME_STATE_LOSE
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
    Player      player;
    EnemyGrid   enemies;
    BulletPool  bullets;
    GameState   state;
    int         score;
    int         screen_width;
    int         screen_height;
} Game;

void game_reset(Game *game, int screen_width, int screen_height);

void game_update(Game *game, float dt);

void game_draw(const Game *game);

#endif 