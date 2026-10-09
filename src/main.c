#include <raylib.h>
#include "player.h"
#include "enemy.h"

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

    if (frameTime > MAX_FRAME_TIME) {
        frameTime = MAX_FRAME_TIME;
    }

    clock->accumulator += frameTime;

    int steps = 0;
    while (clock->accumulator >= FIXED_DT) {
        clock->accumulator -= FIXED_DT;
        steps++;
    }
    return steps;
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(0);

    Player player;
    player_init(&player, SCREEN_WIDTH, SCREEN_HEIGHT);

    EnemyGrid enemies;
    enemy_grid_init(&enemies);

    GameClock clock;
    clock_init(&clock);

    while (!WindowShouldClose()) {
        int steps = clock_tick(&clock);

        for (int i = 0; i < steps; i++) {
            player_update(&player, FIXED_DT, SCREEN_WIDTH);
            enemy_grid_update(&enemies, FIXED_DT, SCREEN_WIDTH);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        player_draw(&player);
        enemy_grid_draw(&enemies);

        DrawFPS(10, 10);
        DrawText("Arrows or A/D to move", 10, 30, 16, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}