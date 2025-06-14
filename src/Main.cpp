#pragma once
#include "Program.hpp"

int main()
{
    Program P;

    int error = 0;

    // Main game loop
    while (P.State != STOPPED) 
    {
        error = P.Run();
    }
    return error;
}