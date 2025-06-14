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

    InitWindow(screenWidth, screenHeight, "Maze Generator with raylib");

    GuiLoadStyleCyber();

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    //ToggleBorderlessWindowed();

    //SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

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

    bool MazeEdit = false;
    bool PathEdit = false;

    // Main program loop
    while (!WindowShouldClose() && State != STOPPED)
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        //check Keys
        if (IsKeyPressed(KEY_M)) {
            if (State == MENU) {
                setState(IDLE);
            }
            else {
                setState(MENU);
                menu.open(*this);
                int i = 0;
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

        ClearBackground(LIGHTGRAY);

        //display buffer
        //DrawTextureRec(buffer.texture, source, Vector2{0, 0}, WHITE);

        //DrawTexturePro(buffer.texture,
        //    source,
        //    Rectangle{ (GetScreenWidth() - ((float)screenWidth * scale)) * 0.5f, (GetScreenHeight() - ((float)screenHeight * scale)) * 0.5f,(float)screenWidth * scale, (float)screenHeight * scale },
        //    Vector2{ 0, 0 },
        //    0.0f,
        //    WHITE);


        DrawTexturePro(
            buffer.texture,
            source,
            Rectangle{ 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight) },
            Vector2{ 0, 0 },
            0,
            WHITE);

        //DrawTexture(buffer.texture, 0, 0, WHITE);

        GuiLabel(Rectangle{ 1000, 10, 200, 24 }, "Maze Generator");
        if (GuiDropdownBox(Rectangle{ 1000, 35, dropdown_length, dropdown_thick }, "REC_BACKTRACKING;KRUSKAL;HUNTANDKILL;CUSTOM", &Maze_GUI, MazeEdit)) {

            std::cout << Maze_GUI << std::endl;
            MazeEdit = !MazeEdit;
        }

        GuiLabel(Rectangle{ 1300, 10, 200, 24 }, "Path Solver");
        if (GuiDropdownBox(Rectangle{ 1300, 35, dropdown_length, dropdown_thick }, "BFS;DFS", &Path_GUI, PathEdit)) {

            std::cout << Path_GUI << std::endl;
            PathEdit = !PathEdit;

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
