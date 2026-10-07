#include <raylib.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 450

// Логика обновляется ровно 60 раз в секунду, независимо от FPS
#define FIXED_DT (1.0f / 60.0f)

// Защита от "спирали смерти": если один кадр занял слишком много времени,
// не даём accumulator'у расти бесконечно
#define MAX_FRAME_TIME 0.25f

typedef struct {
    float accumulator;   // сколько "неотыгранного" времени накопилось
    double lastTime;     // время предыдущего кадра (в секундах)
} GameClock;

static void clock_init(GameClock *clock) {
    clock->accumulator = 0.0f;
    clock->lastTime = GetTime();
}

// Возвращает количество шагов физики, которое нужно выполнить в этом кадре
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

// Заглушка: здесь будет обновляться вся игровая логика
static void game_update(float dt) {
    (void)dt; // пока не используем — но сигнатура готова к будущему
}

static void game_draw(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Space Invaders — fixed timestep", 160, 180, 20, DARKGRAY);
    DrawText("Logic: 60 Hz | Render: as fast as possible", 160, 210, 16, GRAY);

    DrawFPS(10, 10);

    EndDrawing();
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    SetTargetFPS(0);  // 0 = без ограничения, рендер на частоте монитора
                      // (позже можно вернуть 60, если будет мерцание)

    GameClock clock;
    clock_init(&clock);

    while (!WindowShouldClose()) {
        int steps = clock_tick(&clock);

        // Обновляем логику фиксированное число раз
        for (int i = 0; i < steps; i++) {
            game_update(FIXED_DT);
        }

        // Рисуем один раз за кадр
        game_draw();
    }

    CloseWindow();
    return 0;
}