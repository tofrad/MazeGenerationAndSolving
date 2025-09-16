#pragma once
#include "Program.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    Program P;

    int error = 0;

    std::cout << "Current path is " << fs::current_path() << '\n'; // (1)
    fs::current_path(fs::temp_directory_path()); // (3)
    std::cout << "Current path is " << fs::current_path() << '\n';

    // Main game loop
    while (P.getState() != STOPPED) 
    {
        error = P.Run();
    }
    return error;
}