#define RAYGUI_IMPLEMENTATION
#include "../lib/raylib/include/raygui.h"

#include "../lib/raylib/include/RL_Tech.h"

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

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    //ClearWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(FrameRate);

    GuiLoadStyleDefault();
    GuiLoadStyleRLTech();
    // GuiSetStyle("Dark");

    //-------------------------------------------------------------------------------------
    handleGeneratorRequest(MazeWidth, Generator);

    //--------------------------------------------------------------------------------------
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

        if (IsKeyPressed(KEY_THREE)) {
            setState(PLAYER);
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        int window_width = GetScreenWidth();
        int window_height = GetScreenHeight();

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
            // DrawTexturePro(
            //     buffer.texture,
            //     source,
            //     Rectangle{ 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight) },
            //     Vector2{ 0, 0 },
            //     0,
            //     WHITE);
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

void Program::handleGeneratorRequest(const int size, const GenerationMethod method)
{
    CalculateMazeParams(size);

    Generator = method;

    if (Generator == CUSTOM) {

        //forward TileArray from editor to Maze
        if (editor->CustomMaze.isValid) {
            //size params at this point unknown, needs to be set in Maze class
            Gen_Recorder = Recorder(0,0, RecordType::MAZE);
            M = Maze(&editor->CustomMaze, &Gen_Recorder);
            handleSolveRequest(Solver);
        }
        else {
            //TODO
            //throw error and logging
        }
    }
    else{
        Gen_Recorder = Recorder(MazeHeight,MazeWidth, RecordType::MAZE);
        M = Maze(MazeWidth, MazeHeight, Generator, &Gen_Recorder);
        handleSolveRequest(Solver);
    }
}

void Program::handleSolveRequest(const SolvingMethod method)
{
    Solver = method;
    M.resetMaze();

    Solve_Recorder = Recorder(M.getGeneratedMaze(), M.getHeight(), M.getWidth(), RecordType::PATH);
    S = Pathsolver(M.getStart(), Solver, &Solve_Recorder);
    Solve_Recorder.saveLastFrame(M.getGeneratedMaze());
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

        menu->open();

        if (State != STOPPED) {
            LastState = State;
        }
        State = next_state;
        break;

    case EDITING:

        editor->open();
        if (State != STOPPED) {
            LastState = State;
        }
        State = next_state;
        break;

    case PLAYER:
        player->open(&Gen_Recorder, &Solve_Recorder);

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
    callbacks.getMazeSize = [this]() {return MazeWidth; };
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
// void Program::getLastMazeFrame() const
// {
//     BeginTextureMode(buffer);
//     ClearBackground(LIGHTGRAY);
//     DrawTextureRec(last_maze_buffer.texture, source, Vector2{ 0, 0 }, WHITE);
//     EndTextureMode();
// }

// void Program::getLastPathFrame() const
// {
//     BeginTextureMode(buffer);
//     ClearBackground(LIGHTGRAY);
//     DrawTextureRec(last_path_buffer.texture, source, Vector2{ 0, 0 }, WHITE);
//     EndTextureMode();
// }

void Program::CalculateMazeParams(const int width)
{
    if (MazeWidth > MAX_WIDTH) {
    	MazeWidth = MAX_WIDTH;
    }
    else if (MazeWidth < MIN_WIDTH) {
    	MazeWidth = MIN_WIDTH;
    }
    else {
    	MazeWidth = width;
    }
    if (MazeWidth % 2 == 0) {
    	MazeWidth = MazeWidth - 1;
    }

    int temp_height = (MazeWidth* 9) / 16;
    if (temp_height % 2 == 0) {
    	temp_height = temp_height - 1;
    }

    if (temp_height > MAX_HEIGHT) {
    	MazeHeight = MAX_HEIGHT;
    }
    else if (temp_height < MIN_HEIGHT) {
    	MazeHeight = MIN_HEIGHT;
    }
    else {
    	MazeHeight = temp_height;
    }
}

