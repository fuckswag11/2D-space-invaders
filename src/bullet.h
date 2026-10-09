#ifndef BULLET_H
#define BULLET_H

#include <raylib.h>

// Размеры пули
#define BULLET_WIDTH   4
#define BULLET_HEIGHT  12

#define BULLET_SPEED   500.0f

#define BULLET_POOL_SIZE 64

typedef struct {
    Vector2 position;  
    bool    alive;    
} Bullet;

typedef struct {
    Bullet bullets[BULLET_POOL_SIZE];
    int    alive_count; 
} BulletPool;

void bullet_pool_init(BulletPool *pool);

void bullet_pool_spawn(BulletPool *pool, Vector2 origin);

void bullet_pool_update(BulletPool *pool, float dt);

void bullet_pool_draw(const BulletPool *pool);

bool bullet_pool_check_hit(BulletPool *pool, Rectangle target);

#endif 