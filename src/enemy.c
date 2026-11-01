#include "enemy.h"

void enemy_grid_init(EnemyGrid *grid, int level, Texture2D sprite_squid, Texture2D sprite_crab, Texture2D sprite_octopus) {
    grid->direction = 1;
    grid->speed = ENEMY_SPEED + (level - 1) * 20.0f; 
    grid->alive_count = ENEMY_ROWS * ENEMY_COLS;

    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            Enemy *e = &grid->enemies[row][col];
            e->position.x = (float)(col * (ENEMY_WIDTH  + ENEMY_GAP_X));
            float start_y = ENEMY_TOP_MARGIN + (level - 1) * 10.0f; 
            e->position.y = start_y + (float)(row * (ENEMY_HEIGHT + ENEMY_GAP_Y));
            e->alive = true;
            e->type = row;

            if (row == 0) e->sprite = sprite_squid;
            else if (row == 1 || row == 2) e->sprite = sprite_crab;
            else e->sprite = sprite_octopus;
        }
    }
}

static void enemy_grid_bounds(const EnemyGrid *grid, float *min_x, float *max_x) {
    float min = 1e9f;
    float max = -1e9f;
    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            const Enemy *e = &grid->enemies[row][col];
            if (!e->alive) continue;
            if (e->position.x < min) min = e->position.x;
            float right = e->position.x + ENEMY_WIDTH;
            if (right > max) max = right;
        }
    }
    *min_x = min;
    *max_x = max;
}

void enemy_grid_update(EnemyGrid *grid, float dt, int screen_width) {
    if (grid->alive_count == 0) return;
    float dx = grid->speed * grid->direction * dt;
    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            Enemy *e = &grid->enemies[row][col];
            if (!e->alive) continue;
            e->position.x += dx;
        }
    }
    float min_x, max_x;
    enemy_grid_bounds(grid, &min_x, &max_x);
    bool hit_right = (max_x >= screen_width);
    bool hit_left  = (min_x <= 0);
    if ((grid->direction > 0 && hit_right) || (grid->direction < 0 && hit_left)) {
        grid->direction = -grid->direction;
        for (int row = 0; row < ENEMY_ROWS; row++) {
            for (int col = 0; col < ENEMY_COLS; col++) {
                Enemy *e = &grid->enemies[row][col];
                if (!e->alive) continue;
                e->position.y += ENEMY_DROP;
            }
        }
        grid->speed += 5.0f;
    }
}

void enemy_grid_draw(const EnemyGrid *grid) {
    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            const Enemy *e = &grid->enemies[row][col];
            if (!e->alive) continue;
            DrawTextureEx(e->sprite, e->position, 0.0f, 3.0f, WHITE);
        }
    }
}

bool enemy_grid_is_empty(const EnemyGrid *grid) {
    return grid->alive_count == 0;
}

bool enemy_grid_reached_bottom(const EnemyGrid *grid, int y_threshold) {
    for (int row = ENEMY_ROWS - 1; row >= 0; row--) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            const Enemy *e = &grid->enemies[row][col];
            if (!e->alive) continue;
            if (e->position.y + ENEMY_HEIGHT >= y_threshold) return true;
            return false;
        }
    }
    return false;
}