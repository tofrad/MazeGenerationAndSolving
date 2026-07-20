#pragma once

namespace Maze_Config
{
    // maze size limits
    constexpr int MIN_WIDTH = 11;
    constexpr int MIN_HEIGHT = 11;
    constexpr int MAX_WIDTH = 149;
    constexpr int MAX_HEIGHT = 149;

    //maze weight limits

    //maze strings
    const std::string MIN_W_STR = std::to_string(MIN_WIDTH);
    const std::string MAX_W_STR = std::to_string(MAX_WIDTH);

    //Algorithm inputs
    inline const char* GEN_INPUT = "Recursive Backtracking\nKruskal\nHunt&Kill\nCustom";
    inline const char* SOLVE_INPUT = "Breadth-First\nDepth-First";

}
