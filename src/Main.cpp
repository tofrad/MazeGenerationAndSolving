// Maze.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include "raylib.h"

#include "Maze.hpp"
#include "Pathsolver.hpp"


int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    //SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Maze Generator with raylib");

    SetTargetFPS(200);           

    //--------------------------------------------------------------------------------------

    //Test stuff ###############################################################
    int size_x = 70;
    int size_y = (size_x * 9) / 16;

    Maze M = Maze(size_x, size_y, screenWidth, screenHeight);

    M.generateMaze();

    Pathsolver S = Pathsolver(M.getGeneratedMaze());

    S.solveMaze(M.getStart());

    //create buffer for drawing
    RenderTexture2D buffer = LoadRenderTexture(screenWidth, screenHeight);
    Rectangle source = { 0, 0, buffer.texture.width, -buffer.texture.height };
    //BeginTextureMode(buffer);
    //M.drawMaze();
    //EndTextureMode();
    //------##########################################################################

    // Main game loop
    while (!WindowShouldClose()) 
    {
        //write changes into buffer
        BeginTextureMode(buffer);
                
        if (M.playRecording()){
            SetTargetFPS(550);
        }
        else {
            S.playRecording();
            SetTargetFPS(90);
        }

        EndTextureMode();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(WHITE);

        //display buffer
        
        DrawTextureRec(buffer.texture, source, Vector2{0, 0}, WHITE);
        //DrawTexture(buffer.texture, 0, 0, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow(); 

    return 0;
}