#include <raylib.h>
#include "game.h"
#include "sprites.h"
#include "audio_system.h"

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
    SetExitKey(KEY_NULL);
    InitAudioDevice();
    AudioSystem audio;
    audio_system_init(&audio);
    SetTargetFPS(0);
    GameSprites sprites;
    sprites_load(&sprites);
    Game game;
    game.sprites = sprites;
    game_reset(&game, SCREEN_WIDTH, SCREEN_HEIGHT);
    game.state = GAME_STATE_MENU;

    GameClock clock;
    clock_init(&clock);
    
    while (!WindowShouldClose()) {
        if (game_menu_handle_input(&game)) {
            break;
        }
        if (game.state != GAME_STATE_MENU &&
            game.state != GAME_STATE_PAUSED &&
        IsKeyPressed(KEY_R)) {

            game_reset(
                &game,
                SCREEN_WIDTH,
                SCREEN_HEIGHT
            );
        }
        if (IsKeyPressed(KEY_N)) {
            game_next_level(&game);
        }

        if (game.state == GAME_STATE_PLAYING && IsKeyPressed(KEY_SPACE)) {
            Vector2 muzzle = {
                game.player.position.x + PLAYER_WIDTH / 2.0f,
                game.player.position.y
            };
            int bullets_before = game.bullets.alive_count;

            bullet_pool_spawn(&game.bullets, muzzle);

            if (game.bullets.alive_count > bullets_before) {
                game_events_push(
                    &game.events,
                    GAME_EVENT_PLAYER_SHOOT
                );
            }
        }
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (game.state == GAME_STATE_PLAYING) {
                game.state = GAME_STATE_PAUSED;
            }
            else if (game.state == GAME_STATE_PAUSED) {
                game.state = GAME_STATE_PLAYING;
            }
        }
        
        int steps = clock_tick(&clock);

        for (int i = 0; i < steps; i++) {
            game_update(&game, FIXED_DT);
        }
        audio_system_process(&audio, &game.events);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        game_draw(&game, FIXED_DT);

        EndDrawing();
    }

    sprites_unload(&sprites);
    audio_system_unload(&audio);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}