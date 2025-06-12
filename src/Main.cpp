#include <iostream>
#include "raylib.h"

#include "Maze.hpp"
#include "Pathsolver.hpp"


int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1920;
    int screenHeight = 1080;

    //SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Maze Generator with raylib");

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    ToggleBorderlessWindowed();

    SetTargetFPS(200);           

    //--------------------------------------------------------------------------------------

    //Test stuff ---------------------------------------------------------------------------
    int size_x = 100;

    Maze M = Maze(size_x, screenWidth, screenHeight, HUNTANDKILL);

    Pathsolver S = Pathsolver(M.getGeneratedMaze());

    S.solveMaze(M.getStart());

    //create buffer for drawing
    RenderTexture2D buffer = LoadRenderTexture(screenWidth, screenHeight);
    Rectangle source = { 0, 0, buffer.texture.width, -buffer.texture.height };

    //BeginTextureMode(buffer);
    //M.drawMaze();
    //EndTextureMode();
    // 
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) 
    {
        //write changes into buffer
        BeginTextureMode(buffer);
                
        if (M.playRecording()){
            SetTargetFPS(1450);
        }
        else {
            S.playRecording();
            SetTargetFPS(170);
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