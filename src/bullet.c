#include "bullet.h"

void bullet_pool_init(BulletPool *pool) {
    pool->alive_count = 0;
    for (int i = 0; i < BULLET_POOL_SIZE; i++) {
        pool->bullets[i].alive = false;
        pool->bullets[i].position = (Vector2){ 0.0f, 0.0f };
    }
}

void bullet_pool_spawn(BulletPool *pool, Vector2 origin) {
    for (int i = 0; i < BULLET_POOL_SIZE; i++) {
        Bullet *b = &pool->bullets[i];
        if (b->alive) continue;

        b->alive = true;
        b->position.x = origin.x - BULLET_WIDTH / 2.0f;
        b->position.y = origin.y - BULLET_HEIGHT;
        pool->alive_count++;
        return;
    }
}

void bullet_pool_update(BulletPool *pool, float dt) {
    float dy = BULLET_SPEED * dt;

    for (int i = 0; i < BULLET_POOL_SIZE; i++) {
        Bullet *b = &pool->bullets[i];
        if (!b->alive) continue;

        b->position.y -= dy;

        if (b->position.y + BULLET_HEIGHT < 0) {
            b->alive = false;
            pool->alive_count--;
        }
    }
}

void bullet_pool_draw(const BulletPool *pool) {
    for (int i = 0; i < BULLET_POOL_SIZE; i++) {
        const Bullet *b = &pool->bullets[i];
        if (!b->alive) continue;

        DrawRectangle(
            (int)b->position.x,
            (int)b->position.y,
            BULLET_WIDTH,
            BULLET_HEIGHT,
            YELLOW
        );
    }
}

bool bullet_pool_check_hit(BulletPool *pool, Rectangle target) {
    for (int i = 0; i < BULLET_POOL_SIZE; i++) {
        Bullet *b = &pool->bullets[i];
        if (!b->alive) continue;

        Rectangle bullet_rect = {
            b->position.x,
            b->position.y,
            (float)BULLET_WIDTH,
            (float)BULLET_HEIGHT
        };

        if (CheckCollisionRecs(bullet_rect, target)) {
            b->alive = false;
            pool->alive_count--;
            return true;
        }
    }
    return false;
}