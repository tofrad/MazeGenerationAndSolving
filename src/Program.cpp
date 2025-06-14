#pragma once
#include <iostream>
#include "Program.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "style_cyber.h"

Program::Program()
{
	InitProgram();
    State = IDLE;
}

Program::~Program()
{

}

void Program::InitProgram()
{
	//create raylib dependencies
    // 
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(GetScreenWidth(), GetScreenHeight(), "Maze Generator with raylib");

    GuiLoadStyleCyber();

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    ToggleBorderlessWindowed();

    SetTargetFPS(FrameRate);

    //--------------------------------------------------------------------------------------

    M = Maze(MazeSize, screenWidth, screenHeight, HUNTANDKILL);

    S = Pathsolver(M.getGeneratedMaze(), M.getStart(), SM_BFS);

    //create buffer for drawing
    buffer = LoadRenderTexture(screenWidth, screenHeight);
    source = { 0, 0, (float)buffer.texture.width, (float) - buffer.texture.height};

    //--------------------------------------------------------------------------------------

    scale = min((float)GetScreenWidth() / screenWidth, (float)GetScreenHeight() / screenHeight);

}

int Program::Run()
{

    float dropdown_height = 40;
    float dropdown_length = 250;
    float dropdown_thick = 30;

    int Maze_GUI = 0;
    int Path_GUI = 0;

    bool MazeEditMode = false;
    bool PathEditMode = false;

    // Main program loop
    while (!WindowShouldClose() && State != STOPPED)
    {
        //check Keys
        if (IsKeyPressed(KEY_M)) {
            if (State == MENU) {
                setState(IDLE);
            }
            else {
                setState(MENU);
            }  
        }

        if (IsKeyPressed(KEY_ONE)) {

            setState(PLAY_MAZE);
        }

        if (IsKeyPressed(KEY_TWO)) {

            setState(PLAY_PATH);
        }

        //write changes into buffer
        BeginTextureMode(buffer);
        switch (State) {
            case PLAY_MAZE:
                if (!M.playRecording()) {
                    setState(IDLE);
                }
                break;
            case PLAY_PATH:
                if (!S.playRecording()) {
                    setState(IDLE);
                }
                break;
            case MENU:

                break;
        }

        //if (M.playRecording()) {
        //    //SetTargetFPS(40);
        //}
        //else {
        //    S.playRecording();
        //    //SetTargetFPS(170);
        //}

        EndTextureMode();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        //display buffer
        //DrawTextureRec(buffer.texture, source, Vector2{0, 0}, WHITE);

        DrawTexturePro(buffer.texture,
            source,
            Rectangle{ (GetScreenWidth() - ((float)screenWidth * scale)) * 0.5f, (GetScreenHeight() - ((float)screenHeight * scale)) * 0.5f,(float)screenWidth * scale, (float)screenHeight * scale },
            Vector2{ 0, 0 },
            0.0f,
            WHITE);
        //DrawTexture(buffer.texture, 0, 0, WHITE);

        GuiLabel(Rectangle{ 10, 10, 200, 24 }, "Maze Generator");
        if (GuiDropdownBox(Rectangle{ 10, 35, dropdown_length, dropdown_thick }, "REC_BACKTRACKING;KRUSKAL;HUNTANDKILL;CUSTOM", &Maze_GUI, MazeEditMode)) {

            std::cout << Maze_GUI << std::endl;
            MazeEditMode = !MazeEditMode;
        }

        GuiLabel(Rectangle{ 270, 10, 400, 24 }, "Path Solver");
        if (GuiDropdownBox(Rectangle{ 270, 35, dropdown_length, dropdown_thick }, "BFS;DFS", &Path_GUI, PathEditMode)) {

            std::cout << Path_GUI << std::endl;
            PathEditMode = !PathEditMode;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();

    setState(STOPPED);
	return 0;
}

void Program::setState(ProgramState next_state)
{
    switch (next_state) {
        case IDLE:

            break;
        case STOPPED:

            break;
        case MENU:

            break;
        case EDITING:

            break;
        case PLAY_MAZE:

            break;
        case PLAY_PATH:

            break;
        default:
            setState(IDLE);
            break;
    }

    State = next_state;

}
