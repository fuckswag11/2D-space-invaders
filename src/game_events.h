#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H

#include <stdbool.h>

#define GAME_EVENT_CAPACITY 128

typedef enum {
    GAME_EVENT_PLAYER_SHOOT = 0,
    GAME_EVENT_ENEMY_SHOOT,
    GAME_EVENT_ENEMY_HIT,
    GAME_EVENT_PLAYER_HIT,
    GAME_EVENT_SHIELD_HIT,
    GAME_EVENT_UFO_HIT,
    GAME_EVENT_WIN,
    GAME_EVENT_LOSE,
    GAME_EVENT_COUNT
} GameEventType;

typedef struct {
    GameEventType items[GAME_EVENT_CAPACITY];
    int count;
} GameEventQueue;

static inline void game_events_clear(GameEventQueue *queue) {
    queue->count = 0;
}

// Returns false if the bounded queue is full.
static inline bool game_events_push(GameEventQueue *queue, GameEventType event) {
    if (queue->count >= GAME_EVENT_CAPACITY) return false;
    queue->items[queue->count++] = event;
    return true;
}

#endif
