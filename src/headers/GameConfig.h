#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <stdint.h>

namespace GameConfig {
    constexpr double TARGET_FPS = 60.0;
    constexpr uint64_t TARGET_FRAMERATE_NS = (uint64_t)(1000000000.0 / TARGET_FPS);

    constexpr int WINDOW_WIDTH = 1280;
    constexpr int WINDOW_HEIGHT = 720; 

    constexpr int PAUSE_WIDTH = 20;
    constexpr int PAUSE_HEIGHT = 50;

    constexpr int PLAYER_WIDTH = 50;
    constexpr int PLAYER_HEIGHT = 50;
    constexpr double PLAYER_SPEED = 6;
}

#endif // GAME_CONFIG_H
