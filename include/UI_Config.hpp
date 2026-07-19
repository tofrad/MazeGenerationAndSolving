#pragma once

#include "ColorConfig.hpp"
#include "raylib.h"

namespace UI_Config
{
    //colors
    constexpr Color WEIGHT_90_COLOR = HALF_RED;
    constexpr Color WEIGHT_80_COLOR = LIGHT_RED;
    constexpr Color WEIGHT_70_COLOR = LIGHT_ORANGE;
    constexpr Color WEIGHT_60_COLOR = LIGHT_YELLOW;
    constexpr Color WEIGHT_50_COLOR = LIGHT_LIMEGREEN;
    constexpr Color WEIGHT_40_COLOR = LIGHT_GREEN;
    constexpr Color WEIGHT_30_COLOR = LIGHT_TURQUOISE;
    constexpr Color WEIGHT_20_COLOR = LIGHT_BLUE;
    constexpr Color WEIGHT_10_COLOR = FAINT_BLUE;
    constexpr Color WEIGHT_00_COLOR = LIGHT_PURPLE;

    constexpr Color EMPTY_COLOR = LIGHTGRAY;

    constexpr Color START_COLOR = RED;
    constexpr Color TARGET_COLOR = BLUE;
    constexpr Color WALL_COLOR = BLACK;

    constexpr Color MAZE_ACTIVE_COLOR = LIME;
    constexpr Color MAZE_VISITED_COLOR = DARKGRAY;

    constexpr Color PATH_FRONT_COLOR = GOLD;
    constexpr Color PATH_FIN_COLOR = SKYBLUE;
    constexpr Color PATH_CUR_COLOR = PURPLE;
    constexpr Color PATH_VISITED_COLOR = BROWN;

    //text_size & font maybe? if resize is important


}
