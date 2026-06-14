#include <iostream>
#include "Program.hpp"
#include "Menu.hpp"
#include "Editor.hpp"
#include "Player.hpp"

Program::Program()
{
    menu = new Menu();
    editor = new Editor();
    player = new Player();

    const ProgramCallbacks callbacks = createCallbacks();

    InitProgram();
    menu->init(callbacks);
    editor->init(callbacks);
    player->init(callbacks);
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

    //SetWindowState(FLAG_WINDOW_UNDECORATED);

    //SetWindowState(FLAG_WINDOW_RESIZABLE);

    ClearWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(FrameRate);

    //GuiLoadStyle("src//GUI_Style.rgs");

    //--------------------------------------------------------------------------------------

    M = Maze(MazeSize, buffer_width, buffer_height, Generator);
    M.resetMaze();
    S = Pathsolver(M.getGeneratedMaze(), M.getStart(), Solver);

    //create buffer for drawing
    buffer = LoadRenderTexture(buffer_width, buffer_height);
    source = Rectangle{ 0, 0, static_cast<float>(buffer.texture.width), static_cast<float>(-buffer.texture.height)};

    last_maze_buffer = LoadRenderTexture(screenWidth, screenHeight);
    last_path_buffer = LoadRenderTexture(screenWidth, screenHeight);

    //--------------------------------------------------------------------------------------

    scale = min(static_cast<float>(GetScreenWidth()) / screenWidth, static_cast<float>(GetScreenHeight()) / screenHeight);
}

int Program::Run()
{
    // Main program loop
    while (!WindowShouldClose() && State != STOPPED)
    {
        //check Keys
        if (IsKeyPressed(KEY_M)) {
            if (State == MENU) {
                menu->close();
                setState(LastState);
            }
            else {
                setState(MENU);
            }
        }

        if (IsKeyPressed(KEY_E)) {
            if (State == EDITING) {
                editor->close();
                setState(LastState);
            }
            else {
                setState(EDITING);
            }
        }

        if (IsKeyPressed(KEY_ONE)) {

            setState(PLAY_MAZE);
        }

        if (IsKeyPressed(KEY_TWO)) {

            setState(PLAY_PATH);
        }

        if (IsKeyPressed(KEY_THREE)) {

            setState(PLAYER);
            //open just Mazes for now
            player->open(M.getRecording());
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
            if (Generator == CUSTOM) {
                M.displayInitialFrame();
            }
            break;

        case MENU:
            EndTextureMode();
            //menu_buffer = menu.displayGUI();
            break;

        default:
            break;
        }

        EndTextureMode();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        if (State == MENU) {

            menu->displayGUI();
        }
        else if(State == EDITING) {

            editor->displayEditor();
        }
        else if (State == PLAYER) {

            player->displayPlayerGUI();
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

ProgramState Program::getState() const
{
    return State;
}

void Program::handleStateRequest(const ProgramState state)
{
    setState(state);
}

void Program::handleGeneratorRequest(int size, const GenerationMethod method)
{

    Generator = method;
    last_maze_buffer = LoadRenderTexture(screenWidth, screenHeight);

    if (Generator == CUSTOM) {

        //get TileArray from Editor
        //TileMap currentEditorMaze = editor->CustomMaze;

        //forward TileArray to Maze
        if (editor->CustomMaze.isValid) {
            M = Maze(buffer_width, buffer_height, &editor->CustomMaze);
        }
        else {
            //TODO
            //throw error and logging
        }

    }
    else{
        M = Maze(MazeSize, buffer_width, buffer_height, Generator);
    }


}

void Program::handleSolveRequest(const SolvingMethod method)
{
    Solver = method;
    M.resetMaze();

    //clear prev buffer
    last_path_buffer = LoadRenderTexture(screenWidth, screenHeight);

    S = Pathsolver(M.getGeneratedMaze(), M.getStart(), Solver);

}

void Program::handleWindowChange(const Screensize size)
{

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    if (this->Windowsize == size) {
        ClearWindowState(FLAG_WINDOW_RESIZABLE);
        return;
    }
    else {
        switch (size) {
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
            handleWindowChange(SMALL);
            break;
        }
        SetWindowState(FLAG_WINDOW_RESIZABLE);

        SetWindowSize(screenWidth, screenHeight);

        centerWindow();

        ClearWindowState(FLAG_WINDOW_RESIZABLE);

    }
    ClearWindowState(FLAG_WINDOW_RESIZABLE);
}

void Program::setState(const ProgramState next_state)
{
    switch (next_state) {

    case STOPPED:
        State = next_state;
        break;

    case MENU:
        saveLastFrame();
        menu->open();

        if (State != STOPPED) {
            LastState = State;
        }
        State = next_state;
        break;

    case EDITING:
        saveLastFrame();
        editor->open();
        if (State != STOPPED) {
            LastState = State;
        }
        State = next_state;
        break;

    case PLAY_MAZE:
        saveLastFrame();
        getLastMazeFrame();
        State = next_state;
        break;

    case PLAY_PATH:
        saveLastFrame();
        getLastPathFrame();
        State = next_state;
        break;

    case PLAYER:
        saveLastFrame();
        if (State != STOPPED) {
            LastState = State;
        }
        State = next_state;
        break;

    default:
        setState(MENU);
        break;
    }



}

ProgramCallbacks Program::createCallbacks()
{
    ProgramCallbacks callbacks;

    callbacks.onGenerateRequest = [this](const int size, const GenerationMethod method) {

        this->handleGeneratorRequest(size, method);
    };

    callbacks.onSolveRequest = [this](const SolvingMethod method) {
        this->handleSolveRequest(method);
    };

    callbacks.onWindowRequest = [this](const Screensize size) {
        this->handleWindowChange(size);
    };

    callbacks.onStateRequest = [this](const ProgramState state) {
        this->handleStateRequest(state);
    };


    callbacks.getGenerator = [this]() {return Generator; };
    callbacks.getMazeSize = [this]() {return MazeSize; };
    callbacks.getSolver = [this]() {return Solver; };
    callbacks.getWindowSize = [this]() {return Windowsize; };

    return callbacks;
}

void Program::centerWindow() const
{

    const int monitor = GetCurrentMonitor();
    const int monitor_width = GetMonitorWidth(monitor);
    const int monitor_height = GetMonitorHeight(monitor);
    SetWindowPosition((int)(monitor_width / 2) - (int)(screenWidth / 2), (int)(monitor_height / 2) - (int)(screenHeight / 2));
}

void Program::saveLastFrame() const
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

void Program::getLastMazeFrame() const
{
    BeginTextureMode(buffer);
    ClearBackground(LIGHTGRAY);
    DrawTextureRec(last_maze_buffer.texture, source, Vector2{ 0, 0 }, WHITE);
    EndTextureMode();
}

void Program::getLastPathFrame() const
{
    BeginTextureMode(buffer);
    ClearBackground(LIGHTGRAY);
    DrawTextureRec(last_path_buffer.texture, source, Vector2{ 0, 0 }, WHITE);
    EndTextureMode();
}

