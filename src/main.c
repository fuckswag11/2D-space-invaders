#include <raylib.h>
#include "game.h"

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

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(0);

    Game game;
    game_reset(&game, SCREEN_WIDTH, SCREEN_HEIGHT);

    GameClock clock;
    clock_init(&clock);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            game_reset(&game, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        if (game.state == GAME_STATE_PLAYING && IsKeyPressed(KEY_SPACE)) {
            Vector2 muzzle = {
                game.player.position.x + PLAYER_WIDTH / 2.0f,
                game.player.position.y
            };
            bullet_pool_spawn(&game.bullets, muzzle);
        }

        int steps = clock_tick(&clock);

        for (int i = 0; i < steps; i++) {
            game_update(&game, FIXED_DT);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        game_draw(&game);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}