#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <raylib.h>
#include "game_events.h"

typedef struct {
    Sound sounds[GAME_EVENT_COUNT];
    bool loaded[GAME_EVENT_COUNT];
} AudioSystem;

// Call after InitAudioDevice(). Working directory should be project root.
void audio_system_init(AudioSystem *audio);
void audio_system_process(AudioSystem *audio, GameEventQueue *events);
void audio_system_unload(AudioSystem *audio);

#endif
