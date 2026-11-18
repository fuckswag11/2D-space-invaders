#include "audio_system.h"

static const char *const SOUND_PATHS[GAME_EVENT_COUNT] = {
    "assets/sfx/shoot.wav",
    "assets/sfx/enemy_shoot.wav",
    "assets/sfx/enemy_hit.wav",
    "assets/sfx/player_hit.wav",
    "assets/sfx/shield_hit.wav",
    "assets/sfx/ufo_hit.wav",
    "assets/sfx/win.wav",
    "assets/sfx/lose.wav"
};

static const float SOUND_VOLUMES[GAME_EVENT_COUNT] = {
    0.40f, 0.30f, 0.58f, 0.65f,
    0.30f, 0.60f, 0.65f, 0.65f
};

void audio_system_init(AudioSystem *audio) {
    *audio = (AudioSystem){0};

    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "Audio device is not available");
        return;
    }

    for (int i = 0; i < GAME_EVENT_COUNT; i++) {
        if (!FileExists(SOUND_PATHS[i])) {
            TraceLog(LOG_WARNING, "Missing sound: %s", SOUND_PATHS[i]);
            continue;
        }
        Sound sound = LoadSound(SOUND_PATHS[i]);
        if (IsSoundValid(sound)) {
            audio->sounds[i] = sound;
            audio->loaded[i] = true;
            SetSoundVolume(sound, SOUND_VOLUMES[i]);
        } else {
            TraceLog(LOG_WARNING, "Cannot load sound: %s", SOUND_PATHS[i]);
        }
    }
}

void audio_system_process(AudioSystem *audio, GameEventQueue *events) {
    for (int i = 0; i < events->count; i++) {
        GameEventType type = events->items[i];
        if (type >= 0 && type < GAME_EVENT_COUNT && audio->loaded[type]) {
            PlaySound(audio->sounds[type]);
        }
    }
    game_events_clear(events);
}

void audio_system_unload(AudioSystem *audio) {
    for (int i = 0; i < GAME_EVENT_COUNT; i++) {
        if (audio->loaded[i]) {
            UnloadSound(audio->sounds[i]);
            audio->loaded[i] = false;
        }
    }
}
