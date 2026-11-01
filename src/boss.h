#ifndef BOSS_H
#define BOSS_H
#include <raylib.h>

#define BOSS_WIDTH 60
#define BOSS_HEIGHT 40
#define BOSS_MAX_HP 20

typedef struct {
    Vector2 position;
    int hp;
    bool alive;
    float angle;      
    float shoot_timer;
} Boss;

void boss_init(Boss *boss, int screen_width);
void boss_update(Boss *boss, float dt, int screen_width, int screen_height);
void boss_draw(const Boss *boss);

#endif