#include "enemy.h"

void enemy_grid_init(EnemyGrid *grid) {
    grid->direction = 1;        
    grid->speed = ENEMY_SPEED;
    grid->alive_count = ENEMY_ROWS * ENEMY_COLS;

    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            Enemy *e = &grid->enemies[row][col];
            e->position.x = (float)(col * (ENEMY_WIDTH  + ENEMY_GAP_X));
            e->position.y = (float)(ENEMY_TOP_MARGIN + row * (ENEMY_HEIGHT + ENEMY_GAP_Y));
            e->alive = true;
            e->type = row;       
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

            Color color;
            switch (e->type) {
                case 0:  color = RED;    break;   
                case 1:  color = ORANGE; break;
                case 2:  color = PURPLE; break;
                default: color = MAROON; break;   
            }

            DrawRectangle(
                (int)e->position.x,
                (int)e->position.y,
                ENEMY_WIDTH,
                ENEMY_HEIGHT,
                color
            );
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
            if (e->position.y + ENEMY_HEIGHT >= y_threshold) {
                return true;
            }
            return false;
        }
    }
    return false;
}