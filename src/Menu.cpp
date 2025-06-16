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
}

void Menu::init(Program& P)
{
    //GuiLoadStyleCyber();
    program = &P;
}

void Menu::close()
{
    state = CLOSED;

}

void Menu::displayGUI()
{
    if (state == OPEN) {

        ClearBackground(DARKGRAY);

        GuiLabel(Rectangle{ 1000, 10, 200, 24 }, "Maze Generator");
        if (GuiDropdownBox(Rectangle{ 1000, 35, dropdown_length, dropdown_thick }, "BACKTRACKING;KRUSKAL;HUNTANDKILL;CUSTOM", &Maze_GUI, MazeEdit)) {

            MazeEdit = !MazeEdit;
        }

        GuiLabel(Rectangle{ 1300, 10, 200, 24 }, "Path Solver");
        if (GuiDropdownBox(Rectangle{ 1300, 35, dropdown_length, dropdown_thick }, "BFS;DFS", &Path_GUI, PathEdit)) {

            PathEdit = !PathEdit;
        }
    }
}

void Menu::requestStateChange(ProgramState newState)
{
    if (program != nullptr) {

        program->setState(newState);
    }

}

void Menu::generatorRequest()
{
    if (program != nullptr) {

        program->updateMaze(MazeSize, MazeMethod); 
    }
    

}

void Menu::solverRequest()
{
    if (program != nullptr) {

        program->updatePath(PathMethod); 
    }

    
}
