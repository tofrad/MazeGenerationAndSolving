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
    const int screenWidth = 1300;
    const int screenHeight = 1300;

    //SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "raylib Maze Generator with raylib");

    SetTargetFPS(200);           

    //--------------------------------------------------------------------------------------

    //Test stuff ###############################################################

    Maze M = Maze(80, 80, screenWidth, screenHeight);
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
        BeginTextureMode(buffer);

        //write changes into buffer
        if (M.playRecording()){
            SetTargetFPS(2500);
        }
        else {
            S.playRecording();
            SetTargetFPS(50);
        }

        EndTextureMode();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

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