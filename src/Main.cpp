#include "../include/Program.hpp"

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

//TODO
// Round out UI
//Rescale UI functions while remain static record buffer size
//

//TODO
// Fix Custom Maze Handling in Recorder and/or Player
//draws prev maze when pressing play when there should just be a initial frame
//no drawing apart from initial in path


//TODO
// Prepare Code for WebAssembly

//TODO
// Implement Program Element Base Class for Menu, Editor, Player
//
