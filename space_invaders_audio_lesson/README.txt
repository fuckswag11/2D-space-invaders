SPACE INVADERS - AUDIO EVENT SYSTEM (C / raylib 5.5)

Copy folders src/ and assets/ into the root of your 2D-space-invaders project.
These files ADD new files only; they do not replace any existing ones.

Files supplied:
  src/game_events.h
  src/audio_system.h
  src/audio_system.c
  assets/sfx/*.wav (8 original chiptune effects)

Integration required in existing project files:

1) src/game.h
   #include "game_events.h"
   Inside struct Game: GameEventQueue events;

2) src/game.c
   Inside game_reset(): game_events_clear(&game->events);
   Add game_events_push(&game->events, GAME_EVENT_...);
   at actual collision/win/lose/shield/UFO/enemy shot events.

3) src/main.c
   #include "audio_system.h"
   After InitWindow(): InitAudioDevice();
   After InitAudioDevice():
       AudioSystem audio;
       audio_system_init(&audio);
   After running fixed-timestep game_update calls (before BeginDrawing()):
       audio_system_process(&audio, &game.events);
   During player shooting, after successful bullet_pool_spawn():
       game_events_push(&game.events, GAME_EVENT_PLAYER_SHOOT);
   Before CloseWindow():
       audio_system_unload(&audio);
       CloseAudioDevice();

4) CMakeLists.txt
   Add src/audio_system.c to add_executable(...).

5) Build from project root:
   cmake --preset conan-release
   cmake --build --preset conan-release
   ./build/space_invaders

See the accompanying ChatGPT tutorial for precise insertion points and cautions.
All 8 WAV files here are original, generated for the tutorial.
