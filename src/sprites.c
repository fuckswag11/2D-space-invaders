#include "sprites.h"
#include <stdlib.h>
#include <raylib.h>

static Texture2D create_texture_from_map(int width, int height, const char **pixels, Color main_color, Color outline_color) {
    Color *data = (Color *)malloc(width * height * sizeof(Color));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char c = pixels[y][x];
            Color c_color = BLANK; 
            if (c == '#') c_color = main_color;
            if (c == 'o') c_color = outline_color;
            data[y * width + x] = c_color;
        }
    }

    Image img = {
        .data = data,
        .width = width,
        .height = height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };
    
    Texture2D tex = LoadTextureFromImage(img);
    
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);

    free(data); 
    return tex;
}

void sprites_load(GameSprites *sprites) {
    const char *player_map[] = {
        "......#......",
        ".....###.....",
        "....#####....",
        "....#####....",
        "#############",
        "..#########..",
        "#############",
        "....#####...."
    };
    sprites->player = create_texture_from_map(13, 8, player_map, SKYBLUE, BLUE);

    // --- ВРАГ 1: Спрут (Верхний ряд) ---
    const char *squid_map[] = {
        "...##...##...",
        "..####.####..",
        ".###.###.###.",
        "###..###..###",
        "####.....####",
        "#############",
        "..###...###..",
        ".##.......##."
    };
    sprites->enemy_squid = create_texture_from_map(13, 8, squid_map, PURPLE, DARKPURPLE);

    // --- ВРАГ 2: Краб (Средний ряд) ---
    const char *crab_map[] = {
        "..#.......#..",
        "...#.....#...",
        "..#########..",
        ".##..###..##.",
        "###..###..###",
        "#############",
        "##.#######.##",
        "#.#.......#.#"
    };
    sprites->enemy_crab = create_texture_from_map(13, 8, crab_map,  RED, MAROON);

    // --- ВРАГ 3: Осьминог (Нижний ряд) ---
    const char *octopus_map[] = {
        "....#####....",
        ".###########.",
        "##..#####..##",
        "##..#####..##",
        "#############",
        "..##.....##..",
        ".#.#.....#.#.",
        "#...........#"
    };
    sprites->enemy_octopus = create_texture_from_map(13, 8, octopus_map, PINK, MAGENTA);

    // --- ПУЛЯ ---
    const char *bullet_map[] = {
        ".#.",
        "#.#",
        "#.#",
        ".#."
    };
    sprites->bullet = create_texture_from_map(3, 4, bullet_map, RED, ORANGE);

    for (int i = 0; i < MAX_STARS; i++) {
        sprites->stars[i].position.x = (float)GetRandomValue(0, 800);
        sprites->stars[i].position.y = (float)GetRandomValue(0, 450);
        sprites->stars[i].size = (float)GetRandomValue(1, 3);
        sprites->stars[i].brightness = (float)GetRandomValue(0, 100) / 100.0f;
        sprites->stars[i].speed = (float)GetRandomValue(10, 50) / 100.0f;
    }
}

void sprites_unload(GameSprites *sprites) {
    UnloadTexture(sprites->player);
    UnloadTexture(sprites->enemy_squid);
    UnloadTexture(sprites->enemy_crab);
    UnloadTexture(sprites->enemy_octopus);
    UnloadTexture(sprites->bullet);
}

void sprites_update_stars(GameSprites *sprites, float dt) {
    for (int i = 0; i < MAX_STARS; i++) {
        sprites->stars[i].brightness += sprites->stars[i].speed * dt;
        if (sprites->stars[i].brightness > 1.0f) {
            sprites->stars[i].brightness = 1.0f;
            sprites->stars[i].speed = -sprites->stars[i].speed;
        } else if (sprites->stars[i].brightness < 0.1f) {
            sprites->stars[i].brightness = 0.1f;
            sprites->stars[i].speed = -sprites->stars[i].speed;
        }
    }
}

void sprites_draw_stars(const GameSprites *sprites, int screen_width, int screen_height) {
    for (int i = 0; i < MAX_STARS; i++) {
        const Star *s = &sprites->stars[i];
        
        unsigned char c = (unsigned char)(s->brightness * 255);
        Color star_color = { c, c, c, 255 };
        
        DrawRectangle((int)s->position.x, (int)s->position.y, (int)s->size, (int)s->size, star_color);
    }
}