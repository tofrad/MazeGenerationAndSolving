#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "../lib/raylib/include/RL_Tech.h"

#include "Program.hpp"

#include <chrono>
#include <iostream>
#include <thread>

#include "Menu.hpp"
#include "Editor.hpp"
#include "Player.hpp"
#include "Maze_Config.hpp"

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

    InitWindow(1600, 900, "Maze Generator with raylib");

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
        if (editor->getCustomMaze()->isValid) {
            //size params at this point unknown, needs to be set in Maze class
            Gen_Recorder.init(0,0, RecordType::MAZE);
            M = Maze(editor->getCustomMaze(), &Gen_Recorder);
            handleSolveRequest(Solver);
        }
        else {
            //TODO
            //throw error and logging
        }
    }
    else{
        Gen_Recorder.init(MazeHeight,MazeWidth, RecordType::MAZE);
        M = Maze(MazeWidth, MazeHeight, Generator, &Gen_Recorder);
        handleSolveRequest(Solver);
    }
}

void Program::handleSolveRequest(const SolvingMethod method)
{
    Solver = method;
    M.resetMaze();

    Solve_Recorder.init(M.getHeight(), M.getWidth(), RecordType::PATH);
    Solve_Recorder.saveInitialFrame(M.getGeneratedMaze());
    S = Pathsolver(M.getStart(), Solver, &Solve_Recorder);
    Solve_Recorder.saveLastFrame(M.getGeneratedMaze());
}

void Program::handleCustomGenerateRequest(TileMap& custom, const GenerationMethod GenMethod)
{
    const auto start = std::chrono::system_clock::now();
    const int maze_size = custom.size;
    const int maze_height = custom.height;

    const Maze temp_maze = Maze(maze_size, maze_height, GenMethod, nullptr);
    temp_maze.GetTileMapFromMaze(custom);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << " seconds" << std::endl;
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

    callbacks.onStateRequest = [this](const ProgramState state) {
        this->handleStateRequest(state);
    };

    callbacks.onCustomGenerateRequest = [this](TileMap& Custom, GenerationMethod GenMethod){
        this->handleCustomGenerateRequest(Custom, GenMethod);
    };

    callbacks.getGenerator = [this]() {return Generator; };
    callbacks.getMazeSize = [this]() {return MazeWidth; };
    callbacks.getSolver = [this]() {return Solver; };

    callbacks.getGeneratorRecording = [this]() {return &Gen_Recorder; };
    callbacks.getSolveRecording = [this]() {return &Solve_Recorder; };

    return callbacks;
}

void Program::CalculateMazeParams(const int width)
{
    MazeWidth = width;
    if (MazeWidth > Maze_Config::MAX_WIDTH) {
    	MazeWidth = Maze_Config::MAX_WIDTH;
    }
    else if (MazeWidth < Maze_Config::MIN_WIDTH) {
    	MazeWidth = Maze_Config::MIN_WIDTH;
    }
    if (MazeWidth % 2 == 0) {
    	MazeWidth = MazeWidth - 1;
    }

    int temp_height = (MazeWidth* 9) / 16;
    if (temp_height % 2 == 0) {
    	temp_height = temp_height - 1;
    }

    if (temp_height > Maze_Config::MAX_HEIGHT) {
    	MazeHeight = Maze_Config::MAX_HEIGHT;
    }
    else if (temp_height < Maze_Config::MIN_HEIGHT) {
    	MazeHeight = Maze_Config::MIN_HEIGHT;
    }
    else {
    	MazeHeight = temp_height;
    }
}

