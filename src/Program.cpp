#pragma once
#include <iostream>
#include "Program.hpp"

Program::Program()
{
	InitProgram();
    setState(MENU);
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

    menu.init(*this);

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

    last_maze_buffer = LoadRenderTexture(screenWidth, screenHeight);
    last_path_buffer = LoadRenderTexture(screenWidth, screenHeight);

    //--------------------------------------------------------------------------------------

    scale = min((float)GetScreenWidth() / screenWidth, (float)GetScreenHeight() / screenHeight);

}

int Program::Run()
{
    // Main program loop
    while (!WindowShouldClose() && State != STOPPED)
    {

        //write changes into buffer
        BeginTextureMode(buffer);

        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        //check Keys
        if (IsKeyPressed(KEY_M)) {
            if (State == MENU) {
                menu.close();
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

        switch (State) {

            case IDLE:
                ClearBackground(LIGHTGRAY);
                break;

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
                menu.displayGUI();
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
    saveLastFrame();
    switch (next_state) {
        case IDLE:

            break;

        case STOPPED:

            break;

        case MENU:
            
            menu.open();
            break;

        case EDITING:

            break;

        case PLAY_MAZE:
            getLastMazeFrame();
            break;

        case PLAY_PATH:
            getLastPathFrame();
            break;

        default:
            setState(IDLE);
            break;
    }

    State = next_state;

}

void Program::saveLastFrame()
{  
    if (State == PLAY_MAZE) {
        BeginTextureMode(last_maze_buffer);
        ClearBackground(LIGHTGRAY);
        DrawTextureRec(buffer.texture, source, Vector2{ 0, 0 }, WHITE);
        EndTextureMode();

    }else if (State == PLAY_MAZE) {
        BeginTextureMode(last_path_buffer);
        ClearBackground(LIGHTGRAY);
        DrawTextureRec(buffer.texture, source, Vector2{ 0, 0 }, WHITE);
        EndTextureMode();
    }
    else {
        //no relevant case yet
    }
     
}

void Program::getLastMazeFrame()
{
    ClearBackground(LIGHTGRAY);
    DrawTextureRec(last_maze_buffer.texture, source, Vector2{ 0, 0 }, WHITE);
}

void Program::getLastPathFrame()
{
    ClearBackground(LIGHTGRAY);
    DrawTextureRec(last_path_buffer.texture, source, Vector2{ 0, 0 }, WHITE);
}
