#include "boss.h"
#include <math.h> 

void boss_init(Boss *boss, int screen_width) {
    boss->hp = BOSS_MAX_HP;
    boss->alive = true;
    boss->angle = 0.0f;
    boss->position = (Vector2){ screen_width / 2.0f, 100.0f };
    boss->shoot_timer = 0.0f;
}

void boss_update(Boss *boss, float dt, int screen_width, int screen_height) {
    if (!boss->alive) return;

    boss->angle += 1.5f * dt;
    boss->position.x = (screen_width / 2.0f) + cosf(boss->angle) * 200.0f;
    boss->position.y = 100.0f + sinf(boss->angle) * 50.0f;

    boss->shoot_timer += dt;
}

void boss_draw(const Boss *boss) {
    if (!boss->alive) return;
    DrawRectangle((int)boss->position.x, (int)boss->position.y, BOSS_WIDTH, BOSS_HEIGHT, MAGENTA);
    float hp_percent = (float)boss->hp / BOSS_MAX_HP;
    DrawRectangle((int)boss->position.x, (int)boss->position.y - 10, (int)(BOSS_WIDTH * hp_percent), 5, RED);
}