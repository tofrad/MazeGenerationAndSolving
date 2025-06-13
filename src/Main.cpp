#include "Program.hpp"


int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    Program P;

    // Main game loop
    while (P.State != STOPPED) 
    {
        P.Run();
    }
    return 0;
}