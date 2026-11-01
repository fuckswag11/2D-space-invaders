#ifndef SPRITES_H
#define SPRITES_H

#include <raylib.h>

#define MAX_STARS 150 

typedef struct {
    Vector2 position;
    float   size;
    float   brightness;
    float   speed;
} Star;

typedef struct {
    Texture2D player;
    Texture2D enemy_squid;  
    Texture2D enemy_crab;   
    Texture2D enemy_octopus;
    Texture2D bullet;
    Star stars[MAX_STARS];
} GameSprites;

void sprites_load(GameSprites *sprites);
void sprites_unload(GameSprites *sprites);

void sprites_update_stars(GameSprites *sprites, float dt);
void sprites_draw_stars(const GameSprites *sprites, int screen_width, int screen_height);

#endif 