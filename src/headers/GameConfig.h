#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <stdint.h>

namespace GameConfig {
    constexpr double TARGET_FPS = 60.0;
    constexpr uint64_t TARGET_FRAMERATE_NS = (uint64_t)(1000000000.0 / TARGET_FPS);

    constexpr int WINDOW_WIDTH = 1280;
    constexpr int WINDOW_HEIGHT = 720; 

    constexpr double PAUSE_WIDTH = 20;
    constexpr double PAUSE_HEIGHT = 50;

    constexpr double PLAYER_WIDTH = 50.0;
    constexpr double PLAYER_HEIGHT = 50.0;
    constexpr double PLAYER_SPEED = 6.0;

    constexpr double MAX_ACCELERATION = 1.0;
	constexpr double ACCELERATION = 0.03;
    constexpr double ACCELERATION_BREAK_COEF = 0.6;
	constexpr double ACCELERATION_EPSILON = 0.001;
}

#endif // GAME_CONFIG_H
