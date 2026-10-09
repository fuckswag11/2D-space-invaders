#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>

#define ENEMY_WIDTH   30
#define ENEMY_HEIGHT  20

#define ENEMY_ROWS    4
#define ENEMY_COLS    8

#define ENEMY_GAP_X   15
#define ENEMY_GAP_Y   15

#define ENEMY_TOP_MARGIN 60

#define ENEMY_SPEED   80.0f

#define ENEMY_DROP    20.0f

typedef struct {
    Vector2 position;   
    bool    alive;      
    int     type;       
} Enemy;

typedef struct {
    Enemy   enemies[ENEMY_ROWS][ENEMY_COLS];
    int     direction;      
    float   speed;        
    int     alive_count;  
} EnemyGrid;

void enemy_grid_init(EnemyGrid *grid);

void enemy_grid_update(EnemyGrid *grid, float dt, int screen_width);

void enemy_grid_draw(const EnemyGrid *grid);

bool enemy_grid_is_empty(const EnemyGrid *grid);

bool enemy_grid_reached_bottom(const EnemyGrid *grid, int y_threshold);

#endif 