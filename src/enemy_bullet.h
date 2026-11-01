#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H
#include <raylib.h>

#define EB_POOL_SIZE 32
#define EB_SPEED 250.0f
#define EB_WIDTH 4
#define EB_HEIGHT 12

typedef struct {
    Vector2 position;
    bool alive;
} EnemyBullet;

typedef struct {
    EnemyBullet bullets[EB_POOL_SIZE];
} EnemyBulletPool;

void eb_pool_init(EnemyBulletPool *pool);
void eb_pool_spawn(EnemyBulletPool *pool, Vector2 origin);
void eb_pool_update(EnemyBulletPool *pool, float dt, int screen_height);
void eb_pool_draw(const EnemyBulletPool *pool);
bool eb_pool_check_hit_player(EnemyBulletPool *pool, Rectangle player_rect);

#endif