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

    SetWindowState(FLAG_WINDOW_UNDECORATED);

    //SetWindowState(FLAG_WINDOW_RESIZABLE);

    ClearWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(FrameRate);

    //--------------------------------------------------------------------------------------

    M = Maze(MazeSize, buffer_width, buffer_height, Generator);

    S = Pathsolver(M.getGeneratedMaze(), M.getStart(), Solver);

    //create buffer for drawing
    buffer = LoadRenderTexture(buffer_width, buffer_height);
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
        //check Keys
        if (IsKeyPressed(KEY_M)) {
            if (State == MENU) {
                menu.close();
                setState(LastState);
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
        //just write recorder stuff - no setStates to preserve TextureMode
        BeginTextureMode(buffer);
        switch (State) {

            case PLAY_MAZE:
                if (!M.playRecording()) {
                    //Recorder finished
                }
                break;

            case PLAY_PATH:
                if (!S.playRecording()) {
                    //Recorder finished
                }
                break;

            case MENU:
                EndTextureMode();
                //menu_buffer = menu.displayGUI();
                break;
        }

        EndTextureMode();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        if (State == MENU) {

            menu.displayGUI();
        }
        else {
            DrawTexturePro(
                buffer.texture,
                source,
                Rectangle{ 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight) },
                Vector2{ 0, 0 },
                0,
                WHITE);
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

        case STOPPED:

            break;

        case MENU:
            saveLastFrame();
            menu.open();

            if (State != STOPPED) {
                LastState = State; 
            }

            break;

        case EDITING:

            break;

        case PLAY_MAZE:
            saveLastFrame();
            getLastMazeFrame();
            break;

        case PLAY_PATH:
            saveLastFrame();
            getLastPathFrame();
            break;

        default:
            setState(MENU);
            break;
    }

    State = next_state;

}

ProgramState Program::getState()
{
    return State;
}

void Program::updateMaze(int size, int method)
{
    Generator = (GenerationMethod)method;

    M = Maze(MazeSize, buffer_width, buffer_height, Generator);

}

void Program::updatePath(int method)
{
    Solver = (SolvingMethod)method;
    M.resetMaze();
    S = Pathsolver(M.getGeneratedMaze(), M.getStart(), Solver);

}

void Program::updateWindow(int size)
{

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    if (this->Windowsize == (Screensize)size) {
        ClearWindowState(FLAG_WINDOW_RESIZABLE);
        return;
    }
    else {
        switch ((Screensize)size) {
        case UHD:
            screenWidth = 2560;
            screenHeight = 1440;
            Windowsize = UHD;
            break;

        case FHD:
            screenWidth = 1920;
            screenHeight = 1080;
            Windowsize = FHD;
            break;

        case WSXGA:
            screenWidth = 1600;
            screenHeight = 900;
            Windowsize = WSXGA;
            break;

        case SMALL:
            screenWidth = 960;
            screenHeight = 540;
            Windowsize = SMALL;
            break;

        default:
            updateWindow((int)SMALL);
            break;
        }
        SetWindowState(FLAG_WINDOW_RESIZABLE);

        SetWindowSize(screenWidth, screenHeight);

        centerWindow();

        ClearWindowState(FLAG_WINDOW_RESIZABLE);

    }
    ClearWindowState(FLAG_WINDOW_RESIZABLE);
}

void Program::centerWindow() {

    int monitor = GetCurrentMonitor();
    int monitor_width = GetMonitorWidth(monitor); 
    int monitor_height = GetMonitorHeight(monitor); 
    SetWindowPosition((int)(monitor_width / 2) - (int)(screenWidth / 2), (int)(monitor_height / 2) - (int)(screenHeight / 2));
}

void Program::saveLastFrame()
{  
    //Checks old states before new state is set
    if (State == PLAY_MAZE) {
        BeginTextureMode(last_maze_buffer);
        ClearBackground(LIGHTGRAY);
        DrawTextureRec(buffer.texture, source, Vector2{ 0, 0 }, WHITE);
        EndTextureMode();

    }else if (State == PLAY_PATH) {
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
    BeginTextureMode(buffer);
    ClearBackground(LIGHTGRAY);
    DrawTextureRec(last_maze_buffer.texture, source, Vector2{ 0, 0 }, WHITE);
}

void Program::getLastPathFrame()
{
    BeginTextureMode(buffer);
    ClearBackground(LIGHTGRAY);
    DrawTextureRec(last_path_buffer.texture, source, Vector2{ 0, 0 }, WHITE);
}

