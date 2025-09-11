#pragma once

#include "Menu.hpp"
#include "Program.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "../style_cyber.h"

Menu::Menu()
{
    
}

Menu::~Menu()
{

}

void Menu::open()
{
    state = OPEN;
    syncToProgram();
}

void Menu::init(ProgramCallbacks& callbacks)
{
    this->callbacks = callbacks;
    GuiLoadStyle("src//GUI_Style.rgs");
    //program = &P;
    syncToProgram();
}

void Menu::close()
{
    state = CLOSED;
}

void Menu::displayGUI()
{
    ClearBackground(GRAY);

    int window_height = 1080;
    int window_width = 1920;

    Screensize size = callbacks.getWindowSize();
    switch (size) {
        case UHD:
            window_height = 1440;
            window_width = 2560;
            break;
        case FHD:
            window_height = 1080;
            window_width = 1920;
            break;
        case WSXGA:
            window_height = 900;
            window_width = 1600;
            break;
        case SMALL:
            window_height = 540;
            window_width = 960;
            break;
        default:
            window_height = 540;
            window_width = 960;
            break;
    }

    float Labelpos_y = 10;

    float Label_offset = 10;

    float Labelwidth = (window_width - (4*Label_offset)) / 3;
    float Label_height = 24;

    //Center Label
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    if (state == OPEN) {

        float x = Label_offset;
        float y = Labelpos_y;
        
        //GuiListView(Rectangle{ x, y + Label_height + 1, Labelwidth, dropdown_thick}, "BACKTRACKING;KRUSKAL;HUNTANDKILL;CUSTOM", &test , &Maze_GUI);
        // 
        //Terminate Section ##############################################################################################################################################################
        if (GuiButton(Rectangle{window_width - Label_offset - Labelwidth, window_height - Label_offset - 2*Label_height, Labelwidth, Label_height }, "Exit Program")) {

            Exit_Button = true;
        }

        //Generator Section ##############################################################################################################################################################

        if (GuiButton(Rectangle{ x, y + 3 * Label_height, Labelwidth, Label_height }, "Generate")) {

            Generate_Button = true;
        }

        GuiLabel(Rectangle{ x, y, Labelwidth, Label_height }, "Maze Generator");
        if (GuiDropdownBox(Rectangle{ x, y + Label_height + 1, Labelwidth, dropdown_thick }, "BACKTRACKING;KRUSKAL;HUNTANDKILL;CUSTOM", &Maze_GUI, MazeEdit)) {

            MazeEdit = !MazeEdit;
            MazeMethod = static_cast<GenerationMethod>(Maze_GUI);
        }

        //Solver Section #################################################################################################################################################################

        x = x + Labelwidth + Label_offset;

        if (GuiButton(Rectangle{ x, y + 3 * Label_height, Labelwidth, Label_height }, "Solve")) {

            Solve_Button = true;
        }
        
        GuiLabel(Rectangle{ x , y, Labelwidth, Label_height }, "Path Solver");
        if (GuiDropdownBox(Rectangle{ x, y + Label_height + 1, Labelwidth, dropdown_thick }, "BFS;DFS", &Path_GUI, PathEdit)) {

            PathEdit = !PathEdit;
            PathMethod = static_cast<SolvingMethod>(Path_GUI);
        }

        //Window Section ################################################################################################################################################################

        x = x + Labelwidth + Label_offset;

        GuiLabel(Rectangle{ x , y, Labelwidth, Label_height }, "Window Size");
        if (GuiDropdownBox(Rectangle{ x, y + Label_height + 1, Labelwidth, dropdown_thick }, "UHD;FHD;WSXGA;SMALL", &Window_GUI, WindowEdit)) {

            WindowEdit = !WindowEdit;
            WindowSize = static_cast<Screensize>(Window_GUI);
            callbacks.onWindowRequest(WindowSize);
        }

        //Resolve Actions here
        if (Generate_Button) {

            callbacks.onGenerateRequest(MazeSize, MazeMethod);
            callbacks.onSolveRequest(PathMethod);

            Generate_Button = false;
        }

        if (Solve_Button) {

            callbacks.onSolveRequest(PathMethod);

            Solve_Button = false;
        }

        if (Exit_Button) {

            requestStateChange(STOPPED);

            Exit_Button = false;
        }

    }
}

void Menu::syncToProgram()
{
    if (callbacks.getGenerator) {
        MazeMethod = callbacks.getGenerator();
    }
    if (callbacks.getMazeSize) {
        MazeSize = callbacks.getMazeSize();
    }
    if (callbacks.getSolver) {
        PathMethod = callbacks.getSolver();
    }
    if (callbacks.getWindowSize) {
        WindowSize = callbacks.getWindowSize();
    }
    Maze_GUI = static_cast<int>(MazeMethod);
    Path_GUI = static_cast<int>(PathMethod);
    Window_GUI = static_cast<int>(WindowSize);
}

void Menu::requestStateChange(ProgramState newState)
{
    callbacks.onStateRequest(newState);
}