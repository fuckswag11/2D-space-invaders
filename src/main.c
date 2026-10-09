#include <raylib.h>
#include "player.h"
#include "enemy.h"
#include "bullet.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 450

#define FIXED_DT (1.0f / 60.0f)
#define MAX_FRAME_TIME 0.25f

typedef struct {
    float  accumulator;
    double lastTime;
} GameClock;

static void clock_init(GameClock *clock) {
    clock->accumulator = 0.0f;
    clock->lastTime = GetTime();
}

static int clock_tick(GameClock *clock) {
    double now = GetTime();
    float frameTime = (float)(now - clock->lastTime);
    clock->lastTime = now;

    if (frameTime > MAX_FRAME_TIME) frameTime = MAX_FRAME_TIME;

    clock->accumulator += frameTime;

    int steps = 0;
    while (clock->accumulator >= FIXED_DT) {
        clock->accumulator -= FIXED_DT;
        steps++;
    }
    return steps;
}

static void handle_bullet_collisions(BulletPool *bullets, EnemyGrid *enemies) {
    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            Enemy *e = &enemies->enemies[row][col];
            if (!e->alive) continue;

            Rectangle enemy_rect = {
                e->position.x,
                e->position.y,
                (float)ENEMY_WIDTH,
                (float)ENEMY_HEIGHT
            };

            if (bullet_pool_check_hit(bullets, enemy_rect)) {
                e->alive = false;
                enemies->alive_count--;
            }
        }
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(0);

    Player player;
    player_init(&player, SCREEN_WIDTH, SCREEN_HEIGHT);

    EnemyGrid enemies;
    enemy_grid_init(&enemies);

    BulletPool bullets;
    bullet_pool_init(&bullets);

    GameClock clock;
    clock_init(&clock);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            Vector2 muzzle = {
                player.position.x + PLAYER_WIDTH / 2.0f,
                player.position.y
            };
            bullet_pool_spawn(&bullets, muzzle);
        }

        int steps = clock_tick(&clock);

        for (int i = 0; i < steps; i++) {
            player_update(&player, FIXED_DT, SCREEN_WIDTH);
            enemy_grid_update(&enemies, FIXED_DT, SCREEN_WIDTH);
            bullet_pool_update(&bullets, FIXED_DT);
            handle_bullet_collisions(&bullets, &enemies);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        player_draw(&player);
        enemy_grid_draw(&enemies);
        bullet_pool_draw(&bullets);

        DrawFPS(10, 10);
        DrawText("Arrows/A,D to move | Space to shoot", 10, 30, 16, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}