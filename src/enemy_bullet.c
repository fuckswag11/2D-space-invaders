#include "enemy_bullet.h"

void eb_pool_init(EnemyBulletPool *pool) {
    for (int i = 0; i < EB_POOL_SIZE; i++) pool->bullets[i].alive = false;
}

void eb_pool_spawn(EnemyBulletPool *pool, Vector2 origin) {
    for (int i = 0; i < EB_POOL_SIZE; i++) {
        if (pool->bullets[i].alive) continue;
        pool->bullets[i].alive = true;
        pool->bullets[i].position = (Vector2){ origin.x, origin.y };
        return;
    }
}

void eb_pool_update(EnemyBulletPool *pool, float dt, int screen_height) {
    for (int i = 0; i < EB_POOL_SIZE; i++) {
        if (!pool->bullets[i].alive) continue;
        pool->bullets[i].position.y += EB_SPEED * dt;
        if (pool->bullets[i].position.y > screen_height) {
            pool->bullets[i].alive = false;
        }
    }
}

void eb_pool_draw(const EnemyBulletPool *pool) {
    for (int i = 0; i < EB_POOL_SIZE; i++) {
        if (!pool->bullets[i].alive) continue;
        DrawRectangle((int)pool->bullets[i].position.x, (int)pool->bullets[i].position.y, EB_WIDTH, EB_HEIGHT, RED);
    }
}

bool eb_pool_check_hit_player(EnemyBulletPool *pool, Rectangle player_rect) {
    for (int i = 0; i < EB_POOL_SIZE; i++) {
        if (!pool->bullets[i].alive) continue;
        Rectangle eb_rect = { pool->bullets[i].position.x, pool->bullets[i].position.y, EB_WIDTH, EB_HEIGHT };
        if (CheckCollisionRecs(eb_rect, player_rect)) {
            pool->bullets[i].alive = false;
            return true;
        }
    }
    return false;
}